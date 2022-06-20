#include "../app/math/Math.h"
#include "Fluid.cuh"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

namespace app::fluid {

    // See Jos Stam's paper "Real-Time Fluid Dynamics for Games" for the algorithm below

    static const unsigned g_threads = 1024;
    static const unsigned g_blocks  = (g_point_count * g_point_count + g_threads - 1) / g_threads;
    __constant__ unsigned g_n       = g_point_count * g_point_count;

    struct Index {
        uint16_t i;
        uint16_t j;
    };

    /******************** Static function declarations ************************/
    static Index screen_to_array_indices(float x, float y);
    static float clamp_to_zero_one(float val);
    static void  advect(float* current, const float* previous, const float* u, const float* v);
    static void  diffuse(float* current, const float* previous, float weight);
    static void  add_scaled(float* target, const float* addend, float multiplier);
    static void  project(float* u_gpu, float* v_gpu, float* u_previous_gpu, float* v_previous_gpu);

    /******************** CUDA kernel function declarations ************************/
    __global__ void     add_around_point_kernel(float* density, uint16_t i_target, uint16_t j_target, float multiplier, float radius);
    __global__ void     set_to_zero_kernel(float* data);
    __global__ void     decrease_density_kernel(float* density);
    __global__ void     advect_kernel(float* current, const float* previous, const float* u, const float* v, float ratio);
    __global__ void     set_bounds_to_zero_kernel(float* data);
    __global__ void     diffuse_kernel(float* current, const float* previous, float ratio);
    __global__ void     add_scaled_kernel(float* target, const float* addend, float multiplier);
    __global__ void     project_step_3_kernel(float* u_current, float* v_current, const float* u_previous);
    __global__ void     set_circle_kernel(float* density, float* u, float* v);
    __global__ void     project_step_2_kernel(float* u_previous);
    __global__ void     project_step_1_kernel(float* v_previous, const float* u_current, const float* v_current);
    __device__ float    distance_kernel(uint16_t i_1, uint16_t j_1, uint16_t i_2, uint16_t j_2);
    __device__ float    sum_neighbors_kernel(const float* data, uint16_t i, uint16_t j);
    __device__ float    horizontal_difference(const float* data, uint16_t i, uint16_t j);
    __device__ float    vertical_difference(const float* source, uint16_t i, uint16_t j);
    __device__ uint32_t to_linear_index(uint16_t i, uint16_t j);

    /******************** Fluid member function implementations ************************/
    Fluid::Fluid() {
        cudaMalloc(&m_density_cuda, g_point_count * g_point_count * sizeof(float));
        cudaMalloc(&m_density_previous_cuda, g_point_count * g_point_count * sizeof(float));
        cudaMalloc(&m_u_cuda, g_point_count * g_point_count * sizeof(float));
        cudaMalloc(&m_v_cuda, g_point_count * g_point_count * sizeof(float));
        cudaMalloc(&m_u_previous_cuda, g_point_count * g_point_count * sizeof(float));
        cudaMalloc(&m_v_previous_cuda, g_point_count * g_point_count * sizeof(float));
    }

    Fluid::~Fluid() {
        cudaFree(m_density_cuda);
        cudaFree(m_density_previous_cuda);
        cudaFree(m_u_cuda);
        cudaFree(m_v_cuda);
        cudaFree(m_u_previous_cuda);
        cudaFree(m_v_previous_cuda);
    }

    void Fluid::add_density(float x, float y, float multiplier) {
        if (x <= 0 || x >= 1 || y <= 0 || y >= 1)
            return;
        const auto grid_position = screen_to_array_indices(x, y);
        add_around_point_kernel<<<g_blocks, g_threads>>>(m_density_cuda, grid_position.i, grid_position.j, multiplier * g_particle_input, 0.045);
        cudaDeviceSynchronize();
    }

    void Fluid::add_velocity(float x, float y, float dx, float dy) {
        if (x <= 0 || x >= 1 || y <= 0 || y >= 1)
            return;
        const auto grid_position = screen_to_array_indices(x, y);
        add_around_point_kernel<<<g_blocks, g_threads>>>(m_u_previous_cuda, grid_position.i, grid_position.j, g_force_input * dx, 0.045);
        add_around_point_kernel<<<g_blocks, g_threads>>>(m_v_previous_cuda, grid_position.i, grid_position.j, g_force_input * dy, 0.045);
        cudaDeviceSynchronize();
    }

    float Fluid::sample_density_at(float x, float y) const {
        return m_density.sample_at(x, y);
    }

    float Fluid::sample_u_at(float x, float y) const {
        return m_u.sample_at(x, y);
    }

    float Fluid::sample_v_at(float x, float y) const {
        return m_v.sample_at(x, y);
    }

    void Fluid::clear_previous() {
        set_to_zero_kernel<<<g_blocks, g_threads>>>(m_density_previous_cuda);
        set_to_zero_kernel<<<g_blocks, g_threads>>>(m_v_previous_cuda);
        set_to_zero_kernel<<<g_blocks, g_threads>>>(m_u_previous_cuda);
        cudaDeviceSynchronize();
    }

    void Fluid::clear_current() {
        set_to_zero_kernel<<<g_blocks, g_threads>>>(m_density_cuda);
        set_to_zero_kernel<<<g_blocks, g_threads>>>(m_v_cuda);
        set_to_zero_kernel<<<g_blocks, g_threads>>>(m_u_cuda);
        cudaDeviceSynchronize();
    }

    void Fluid::density_step() {
        add_scaled(m_density_cuda, m_density_previous_cuda, g_dt);
        diffuse(m_density_previous_cuda, m_density_cuda, g_dt);
        advect(m_density_cuda, m_density_previous_cuda, m_u_cuda, m_v_cuda);

        decrease_density_kernel<<<g_blocks, g_threads>>>(m_density_cuda);
        cudaDeviceSynchronize();
    }

    void Fluid::velocity_step() {
        add_scaled(m_u_cuda, m_u_previous_cuda, g_dt);
        add_scaled(m_v_cuda, m_v_previous_cuda, g_dt);
        diffuse(m_u_previous_cuda, m_u_cuda, g_viscosity_coefficient);
        diffuse(m_v_previous_cuda, m_v_cuda, g_viscosity_coefficient);
        project(m_u_previous_cuda, m_v_previous_cuda, m_u_cuda, m_v_cuda);
        advect(m_u_cuda, m_u_previous_cuda, m_u_previous_cuda, m_v_previous_cuda);
        advect(m_v_cuda, m_v_previous_cuda, m_u_previous_cuda, m_v_previous_cuda);
        project(m_u_cuda, m_v_cuda, m_u_previous_cuda, m_v_previous_cuda);
    }

    void Fluid::step() {
        velocity_step();
        density_step();
        get_from_gpu();
        clear_previous();
    }

    void Fluid::send_to_gpu() {
        cudaMemcpy(m_density_cuda, m_density.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(m_density_previous_cuda, m_density_previous.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(m_u_cuda, m_u.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(m_v_cuda, m_v.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(m_u_previous_cuda, m_u_previous.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(m_v_previous_cuda, m_v_previous.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
    }

    void Fluid::get_from_gpu() {
        cudaMemcpy(m_density.data(), m_density_cuda, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
        cudaMemcpy(m_density_previous.data(), m_density_previous_cuda, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
        cudaMemcpy(m_u.data(), m_u_cuda, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
        cudaMemcpy(m_v.data(), m_v_cuda, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
        cudaMemcpy(m_u_previous.data(), m_u_previous_cuda, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
        cudaMemcpy(m_v_previous.data(), m_v_previous_cuda, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
    }

    void Fluid::set_circle() {
        set_circle_kernel<<<g_blocks, g_threads>>>(m_density_cuda, m_u_cuda, m_v_cuda);
    }

    void Fluid::set_polygon(size_t n, float intensity) {
        assert(n >= 3);
        assert(intensity >= 0 && intensity <= 1);
        std::vector<math::Point> points;
        for (size_t i = 0; i != n + 1; ++i) {
            float theta = i * 2.0 * M_PI / static_cast<float>(n);
            points.push_back(math::Point{0.5, 0.5} + 0.3 * math::Point{std::sin(theta), std::cos(theta)});
        }

        for (size_t i = 0; i != n; ++i) {
            const auto& point_1     = points[i];
            const auto& point_2     = points[i + 1];
            const auto  lerp_points = 40;
            for (size_t j = 0; j != lerp_points + 1; ++j) {
                const auto point = (j * point_1 + (lerp_points - j) * point_2) * (1.0 / static_cast<float>(lerp_points));
                add_density(point);
                add_velocity(point, intensity * intensity * intensity * math::random_point_centered());
            }
        }
    }

    const Matrix& Fluid::u() const {
        return m_u;
    }

    const Matrix& Fluid::v() const {
        return m_v;
    }

    const Matrix& Fluid::density() const {
        return m_density;
    }

    void Fluid::add_density(const math::Point& point, float multiplier) {
        add_density(point.x, point.y, multiplier);
    }

    void Fluid::add_velocity(const math::Point& point, const math::Direction& direction) {
        add_velocity(point.x, point.y, direction.x, direction.y);
    }

    /******************** Static function implementations ************************/

    static float clamp_to_zero_one(float val) {
        return val < 0.0f ? 0.0f : val > 1.0f ? 1.0f : val;
    }

    static Index screen_to_array_indices(float x, float y) {
        const auto i = static_cast<uint16_t>(clamp_to_zero_one(x) * (g_point_count - 1));
        const auto j = static_cast<uint16_t>(clamp_to_zero_one(y) * (g_point_count - 1));
        return {i, j};
    }

    static void advect(float* current, const float* previous, const float* u, const float* v) {
        advect_kernel<<<g_blocks, g_threads>>>(current, previous, u, v, g_dt * static_cast<float>(g_cell_count - 1));
        cudaDeviceSynchronize();

        set_bounds_to_zero_kernel<<<g_blocks, g_threads>>>(current);
        cudaDeviceSynchronize();
    }

    static void diffuse(float* current, const float* previous, float weight) {
        float ratio = weight * g_diffusion_coefficient * (g_cell_count - 1.0f) * (g_cell_count - 1.0f) / g_point_count;
        for (size_t k = 0; k != 20; ++k) {
            diffuse_kernel<<<g_blocks, g_threads>>>(current, previous, ratio);
            cudaDeviceSynchronize();
        }

        set_bounds_to_zero_kernel<<<g_blocks, g_threads>>>(current);
        cudaDeviceSynchronize();
    }

    static void add_scaled(float* target, const float* addend, const float multiplier) {
        add_scaled_kernel<<<g_blocks, g_threads>>>(target, addend, multiplier);
        cudaDeviceSynchronize();
    }

    static void project(float* u_gpu, float* v_gpu, float* u_previous_gpu, float* v_previous_gpu) {
        set_to_zero_kernel<<<g_blocks, g_threads>>>(u_previous_gpu);
        cudaDeviceSynchronize();

        project_step_1_kernel<<<g_blocks, g_threads>>>(v_previous_gpu, u_gpu, v_gpu);
        cudaDeviceSynchronize();

        for (size_t k = 0; k != 20; ++k) {
            project_step_2_kernel<<<g_blocks, g_threads>>>(u_previous_gpu);
            cudaDeviceSynchronize();

            add_scaled(u_previous_gpu, v_previous_gpu, 0.25);
        }

        project_step_3_kernel<<<g_blocks, g_threads>>>(u_gpu, v_gpu, u_previous_gpu);
        cudaDeviceSynchronize();

        set_bounds_to_zero_kernel<<<g_blocks, g_threads>>>(u_gpu);
        set_bounds_to_zero_kernel<<<g_blocks, g_threads>>>(v_gpu);
        cudaDeviceSynchronize();
    }

    /******************** CUDA kernel function implementations ************************/

    __device__ int diff(uint16_t a, uint16_t b) {
        return a >= b ? static_cast<int>(a - b) : -static_cast<int>(b - a);
    }

    __device__ float distance_kernel(uint16_t i_1, uint16_t j_1, uint16_t i_2, uint16_t j_2) {
        const int d_i = diff(i_1, i_2);
        const int d_j = diff(j_1, j_2);

        return std::sqrt(static_cast<float>(d_i * d_i + d_j * d_j)) / static_cast<float>(g_point_count);
    }

    __global__ void add_around_point_kernel(float* density, uint16_t i_target, uint16_t j_target, float multiplier, float radius) {
        const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index >= g_n)
            return;

        const unsigned i = index / g_point_count;
        const unsigned j = index % g_point_count;
        if (i == 0 || j == 0 || i == g_point_count || j == g_point_count)
            return;

        const auto distance = distance_kernel(i, j, i_target, j_target);
        if (distance > radius)
            return;

        density[index] += (radius - distance) * multiplier / radius;
    }

    __global__ void set_to_zero_kernel(float* data) {
        const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index >= g_n)
            return;

        data[index] = 0;
    }

    __global__ void decrease_density_kernel(float* density) {
        unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index < g_n)
            density[index] *= 0.97;
    }

    __device__ uint32_t to_linear_index(uint16_t i, uint16_t j) {
        return i * g_point_count + j;
    }

    __global__ void advect_kernel(float* current, const float* previous, const float* u, const float* v, float ratio) {
        const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index >= g_n)
            return;

        const unsigned i = index / g_point_count;
        const unsigned j = index % g_point_count;

        const float x_temp    = static_cast<float>(i) - ratio * u[index];
        const float y_temp    = static_cast<float>(j) - ratio * v[index];
        const float upper     = static_cast<float>(g_cell_count) - 0.5f;
        const float x_new     = x_temp < 0.5f ? 0.5f : x_temp > upper ? upper : x_temp;
        const float y_new     = y_temp < 0.5f ? 0.5f : y_temp > upper ? upper : y_temp;
        const int   x_new_int = static_cast<int>(x_new);
        const int   y_new_int = static_cast<int>(y_new);
        const float dt_x      = x_new - static_cast<float>(x_new_int);
        const float dt_y      = y_new - static_cast<float>(y_new_int);
        current[index]        = (1 - dt_x) * ((1 - dt_y) * previous[to_linear_index(x_new_int, y_new_int)] + dt_y * previous[to_linear_index(x_new_int, y_new_int + 1)]) +
                         dt_x * ((1 - dt_y) * previous[to_linear_index(x_new_int + 1, y_new_int)] + dt_y * previous[to_linear_index(x_new_int + 1, y_new_int + 1)]);
    }

    __global__ void set_bounds_to_zero_kernel(float* data) {
        const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index >= g_n)
            return;
        const unsigned i = index / g_point_count;
        const unsigned j = index % g_point_count;
        if (i == 0 || j == 0 || i == g_point_count - 1 || j == g_point_count - 1)
            data[index] = 0;
    }

    __device__ float sum_neighbors_kernel(const float* data, uint16_t i, uint16_t j) {
        assert(i > 0 && j > 0);
        return data[to_linear_index(i - 1, j)] + data[to_linear_index(i + 1, j)] + data[to_linear_index(i, j - 1)] + data[to_linear_index(i, j + 1)];
    }

    __global__ void diffuse_kernel(float* current, const float* previous, float ratio) {
        const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index >= g_n)
            return;

        const unsigned i = index / g_point_count;
        const unsigned j = index % g_point_count;
        if (i == 0 || j == 0 || j >= g_point_count || i >= g_point_count)
            return;

        current[index] = (previous[index] + ratio * sum_neighbors_kernel(current, i, j)) / (1 + 4 * ratio);
    }

    __global__ void add_scaled_kernel(float* target, const float* addend, const float multiplier) {
        const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index >= g_n)
            return;

        target[index] += multiplier * addend[index];
    }

    __device__ float horizontal_difference(const float* data, uint16_t i, uint16_t j) {
        return data[to_linear_index(i, j + 1)] - data[to_linear_index(i, j - 1)];
    }

    __device__ float vertical_difference(const float* source, uint16_t i, uint16_t j) {
        return source[to_linear_index(i + 1, j)] - source[to_linear_index(i - 1, j)];
    }

    __global__ void project_step_3_kernel(float* u_current, float* v_current, const float* u_previous) {
        const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index >= g_n)
            return;

        const unsigned i = index / g_point_count;
        const unsigned j = index % g_point_count;
        if (i == 0 || j == 0 || j >= g_point_count || i >= g_point_count)
            return;

        u_current[to_linear_index(i, j)] -= 0.5f * vertical_difference(u_previous, i, j) / g_cell_length;
        v_current[to_linear_index(i, j)] -= 0.5f * horizontal_difference(u_previous, i, j) / g_cell_length;
    }

    __global__ void set_circle_kernel(float* density, float* u, float* v) {
        const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index >= g_n)
            return;

        const unsigned i = index / g_point_count;
        const unsigned j = index % g_point_count;
        if (i == 0 || j == 0 || j >= g_point_count || i >= g_point_count)
            return;

        const auto distance = distance_kernel(i, j, g_point_count / 2, g_point_count / 2);
        if (distance < 0.3 && distance > 0.2) {
            u[index] *= (2 + std::sin(static_cast<float>(density[index] / 10 + 0.03 * i + j * 0.04))); // this is a hack to get random-esque values here
            v[index] *= (2 + std::cos(static_cast<float>(0.02 * i + j * 0.05 - density[index] / 10))); // this is a hack to get random-esque values here
            density[index] = 300;
        }
    }

    __global__ void project_step_2_kernel(float* u_previous) {
        const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index >= g_n)
            return;

        const unsigned i = index / g_point_count;
        const unsigned j = index % g_point_count;
        if (i == 0 || j == 0 || j >= g_point_count || i >= g_point_count)
            return;

        u_previous[to_linear_index(i, j)] = sum_neighbors_kernel(u_previous, i, j) / 4.0f;
    }

    __global__ void project_step_1_kernel(float* v_previous, const float* u_current, const float* v_current) {
        const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
        if (index >= g_n)
            return;

        const unsigned i = index / g_point_count;
        const unsigned j = index % g_point_count;
        if (i == 0 || j == 0 || j >= g_point_count || i >= g_point_count)
            return;

        v_previous[index] = -0.5f * g_cell_length * (vertical_difference(u_current, i, j) + horizontal_difference(v_current, i, j));
    }

} // namespace app::fluid
