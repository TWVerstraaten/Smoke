#include "Fluid.cuh"

#include <cassert>

namespace app {
    namespace fluid {

        // See Jos Stam's paper "Real-Time Fluid Dynamics for Games" for the algorithm below

        static const unsigned g_threads = 1024;
        static const unsigned g_blocks  = (g_point_count * g_point_count + g_threads - 1) / g_threads;
        __constant__ unsigned g_n       = g_point_count * g_point_count;

        struct Index {
            uint16_t i;
            uint16_t j;
        };

        static Index screen_to_array_indices(float x, float y);
        static float clamp_to_zero_one(float val);

        float Fluid::sample_density_at(float x, float y) const {
            return m_density.sample_at(x, y);
        }

        float Fluid::sample_u_at(float x, float y) const {
            return m_u.sample_at(x, y);
        }

        float Fluid::sample_v_at(float x, float y) const {
            return m_v.sample_at(x, y);
        }

        __device__ int diff(unsigned a, unsigned b) {
            return a >= b ? static_cast<int>(a - b) : -static_cast<int>(b - a);
        }

        __global__ void add_around_point_kernel(float* density, uint16_t i_target, uint16_t j_target, float multiplier, float radius) {
            const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
            if (index >= g_n)
                return;

            const unsigned i = index / g_point_count;
            const unsigned j = index % g_point_count;
            if (i == 0 || j == 0 || i == g_point_count || j == g_point_count)
                return;

            const int d_i = diff(i, i_target);
            const int d_j = diff(j, j_target);

            const float dist = std::sqrt(static_cast<float>(d_i * d_i + d_j * d_j)) / static_cast<float>(g_point_count);
            if (dist > radius)
                return;

            density[index] += (radius - dist) * multiplier / radius;
        }

        void Fluid::add_density(float x, float y, float multiplier) {
            if (x <= 0 || x >= 1 || y <= 0 || y >= 1) {
                return;
            }
            const auto grid_position = screen_to_array_indices(x, y);
            add_around_point_kernel<<<g_blocks, g_threads>>>(m_density_gpu, grid_position.i, grid_position.j, multiplier * g_particle_input, 0.035);
            cudaDeviceSynchronize();
        }

        void Fluid::add_velocity(float x, float y, float dx, float dy) {
            if (x <= 0 || x >= 1 || y <= 0 || y >= 1) {
                return;
            }
            const auto grid_position = screen_to_array_indices(x, y);
            add_around_point_kernel<<<g_blocks, g_threads>>>(m_u_previous_gpu, grid_position.i, grid_position.j, g_force_input * dx, 0.035);
            add_around_point_kernel<<<g_blocks, g_threads>>>(m_v_previous_gpu, grid_position.i, grid_position.j, g_force_input * dy, 0.035);
            cudaDeviceSynchronize();
        }

        __global__ void set_to_zero_kernel(float* data) {
            const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
            if (index >= g_n)
                return;

            data[index] = 0;
        }

        void Fluid::clear_previous() {
            set_to_zero_kernel<<<g_blocks, g_threads>>>(m_density_previous_gpu);
            set_to_zero_kernel<<<g_blocks, g_threads>>>(m_v_previous_gpu);
            set_to_zero_kernel<<<g_blocks, g_threads>>>(m_u_previous_gpu);
            cudaDeviceSynchronize();
        }

        void Fluid::clear_current() {
            set_to_zero_kernel<<<g_blocks, g_threads>>>(m_density_gpu);
            set_to_zero_kernel<<<g_blocks, g_threads>>>(m_v_gpu);
            set_to_zero_kernel<<<g_blocks, g_threads>>>(m_u_gpu);
            cudaDeviceSynchronize();
        }

        __global__ void decrease_density_kernel(float* density) {
            unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
            if (index < g_n)
                density[index] *= 0.98;
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

            current[index] = (previous[index] + ratio * sum_neighbors_kernel(current, i, j)) / (1 + (4 * ratio));
        }

        __global__ void add_scaled_kernel(float* target, const float* addend, const float multiplier) {
            const unsigned index = blockIdx.x * blockDim.x + threadIdx.x;
            if (index >= g_n)
                return;

            target[index] += multiplier * addend[index];
        }

        static void advect(float* current, const float* previous, const float* u, const float* v) {
            advect_kernel<<<g_blocks, g_threads>>>(current, previous, u, v, g_dt * static_cast<float>(g_cell_count - 1));
            cudaDeviceSynchronize();

            set_bounds_to_zero_kernel<<<g_blocks, g_threads>>>(current);
            cudaDeviceSynchronize();
        }

        static void diffuse(float* current, const float* previous, float weight) {
            float ratio = weight * g_diffusion_coefficient * (g_cell_count - 1.0f) * (g_cell_count - 1.0f);
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

        void Fluid::density_step() {
            add_scaled(m_density_gpu, m_density_previous_gpu, g_dt);
            diffuse(m_density_previous_gpu, m_density_gpu, g_dt);
            advect(m_density_gpu, m_density_previous_gpu, m_u_gpu, m_v_gpu);

            decrease_density_kernel<<<g_blocks, g_threads>>>(m_density_gpu);
            cudaDeviceSynchronize();
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

        void project(float* u_gpu, float* v_gpu, float* u_previous_gpu, float* v_previous_gpu) {
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

        void Fluid::velocity_step() {
            add_scaled(m_u_gpu, m_u_previous_gpu, g_dt);
            add_scaled(m_v_gpu, m_v_previous_gpu, g_dt);
            diffuse(m_u_previous_gpu, m_u_gpu, g_viscosity_coefficient);
            diffuse(m_v_previous_gpu, m_v_gpu, g_viscosity_coefficient);
            project(m_u_previous_gpu, m_v_previous_gpu, m_u_gpu, m_v_gpu);
            advect(m_u_gpu, m_u_previous_gpu, m_u_previous_gpu, m_v_previous_gpu);
            advect(m_v_gpu, m_v_previous_gpu, m_u_previous_gpu, m_v_previous_gpu);
            project(m_u_gpu, m_v_gpu, m_u_previous_gpu, m_v_previous_gpu);
        }

        void Fluid::step() {
            velocity_step();
            density_step();
            get_from_gpu();
        }

        void Fluid::send_to_gpu() {
            cudaMemcpy(m_density_gpu, m_density.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
            cudaMemcpy(m_density_previous_gpu, m_density_previous.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
            cudaMemcpy(m_u_gpu, m_u.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
            cudaMemcpy(m_v_gpu, m_v.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
            cudaMemcpy(m_u_previous_gpu, m_u_previous.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
            cudaMemcpy(m_v_previous_gpu, m_v_previous.data(), g_point_count * g_point_count * sizeof(float), cudaMemcpyHostToDevice);
        }

        void Fluid::get_from_gpu() {
            cudaMemcpy(m_density.data(), m_density_gpu, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
            cudaMemcpy(m_density_previous.data(), m_density_previous_gpu, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
            cudaMemcpy(m_u.data(), m_u_gpu, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
            cudaMemcpy(m_v.data(), m_v_gpu, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
            cudaMemcpy(m_u_previous.data(), m_u_previous_gpu, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
            cudaMemcpy(m_v_previous.data(), m_v_previous_gpu, g_point_count * g_point_count * sizeof(float), cudaMemcpyDeviceToHost);
        }

        void Fluid::add_bar() {
            for (size_t i = 0; i != 20; ++i) {
                float r = static_cast<float>(rand()) / static_cast<float>(6.0f * RAND_MAX);
                //            auto [i_min, j_min] = screen_to_array_indices(static_cast<float>(i) / 20.0f, 0.0f);
                //            auto [i_max, j_max] = screen_to_array_indices(static_cast<float>(i + 1) / 2.0f, r);
                //            assert(i_min <= i_max);
                //            assert(j_min <= j_max);
                //            const float du = g_force_input * (static_cast<float>(rand()) / (92230.0f * RAND_MAX)) * ((rand() % 10) > 4 ? -1.0f : 1.0f);
                //            for (size_t w = i_min; w <= i_max; ++w) {
                //                for (size_t h = j_min; h <= j_max; ++h) {
                //                    m_density_previous[w][h] = 0.01f * g_particle_input;
                //                    m_v_previous[w][h]       = 0.0001f * g_force_input * r;
                //                    m_u_previous[w][h]       = du;
                //                }
                //            }
            }
        }

        void Fluid::set_circle() {
            for (size_t i = 0; i != g_point_count - 1; ++i) {
                for (size_t j = 0; j != g_point_count - 1; ++j) {
                    //                if (std::sqrt(tools::math::square(i - g_cell_count / 2) + tools::math::square(j - g_cell_count / 2)) < g_cell_count / 4.0f &&
                    //                    std::sqrt(tools::math::square(i - g_cell_count / 2) + tools::math::square(j - g_cell_count / 2)) > g_cell_count / 4.0f - 4) {
                    //                    m_density[i][j] = 120;
                    //                    m_u[i][j] *= 3;
                    //                    m_v[i][j] *= 3;
                    //                }
                }
            }
        }

        Fluid::Fluid() {
            cudaMalloc(&m_density_gpu, g_point_count * g_point_count * sizeof(float));
            cudaMalloc(&m_density_previous_gpu, g_point_count * g_point_count * sizeof(float));
            cudaMalloc(&m_u_gpu, g_point_count * g_point_count * sizeof(float));
            cudaMalloc(&m_v_gpu, g_point_count * g_point_count * sizeof(float));
            cudaMalloc(&m_u_previous_gpu, g_point_count * g_point_count * sizeof(float));
            cudaMalloc(&m_v_previous_gpu, g_point_count * g_point_count * sizeof(float));
        }

        Fluid::~Fluid() {
            cudaFree(m_density_gpu);
            cudaFree(m_density_previous_gpu);
            cudaFree(m_u_gpu);
            cudaFree(m_v_gpu);
            cudaFree(m_u_previous_gpu);
            cudaFree(m_v_previous_gpu);
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
    } // namespace fluid
} // namespace app
