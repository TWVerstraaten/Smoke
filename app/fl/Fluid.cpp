#include "Fluid.h"

#include "../Math.h"

#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

namespace app::fl {

    // See Jos Stam's paper "Real-Time Fluid Dynamics for Games" for the algorithm below

    static void set_bounds_to_zero(Matrix& matrix) {
        for (int i = 0; i != g_cell_count + 1; ++i) {
            matrix[0][i]            = 0;
            matrix[g_cell_count][i] = 0;
            matrix[i][0]            = 0;
            matrix[i][g_cell_count] = 0;
        }
    }

    static float clamp_to_grid(float value) {
        return math::clamp(value, 0.5f, static_cast<float>(g_cell_count) - 0.5f);
    }

    static float sum_neighbors(Matrix& matrix, int i, int j) {
        return matrix[i - 1][j] + matrix[i + 1][j] + matrix[i][j - 1] + matrix[i][j + 1];
    }

    static void diffuse_thread(Matrix& current, const Matrix& previous, float ratio, int start_i, int end_i, int start_j, int end_j) {
        for (int k = 0; k != 10; ++k) {
            for (int i = start_i; i != end_i; ++i) {
                for (int j = start_j; j != end_j; ++j) {
                    current[i][j] = (previous[i][j] + ratio * sum_neighbors(current, i, j)) / (1 + (4 * ratio));
                }
            }
        }
    }

    static void diffuse(Matrix& current, const Matrix& previous, float dt) {
        float       ratio       = dt * g_diffusion_coefficient * (g_cell_count - 1.0f) * (g_cell_count - 1.0f);
        const auto  half_points = g_cell_count / 2;
        std::thread partial_1(diffuse_thread, std::ref(current), std::ref(previous), ratio, 1, half_points, 1, half_points);
        std::thread partial_2(diffuse_thread, std::ref(current), std::ref(previous), ratio, half_points, g_cell_count - 1, 1, half_points);
        std::thread partial_3(diffuse_thread, std::ref(current), std::ref(previous), ratio, half_points, g_cell_count - 1, half_points, g_cell_count - 1);
        std::thread partial_4(diffuse_thread, std::ref(current), std::ref(previous), ratio, 1, half_points, half_points, g_cell_count - 1);
        partial_1.join();
        partial_2.join();
        partial_3.join();
        partial_4.join();
        set_bounds_to_zero(current);
    }

    static void advect_thread(Matrix& current, const Matrix& previous, const Matrix& u, const Matrix& v, float ratio, int start_i, int end_i, int start_j, int end_j) {
        for (int i = start_i; i != end_i; ++i) {
            for (int j = start_j; j != end_j; ++j) {
                const float x_new     = clamp_to_grid(static_cast<float>(i) - ratio * u[i][j]);
                const float y_new     = clamp_to_grid(static_cast<float>(j) - ratio * v[i][j]);
                const int   x_new_int = static_cast<int>(x_new);
                const int   y_new_int = static_cast<int>(y_new);
                const float dt_x      = x_new - static_cast<float>(x_new_int);
                const float dt_y      = y_new - static_cast<float>(y_new_int);
                current[i][j]         = (1 - dt_x) * ((1 - dt_y) * previous[x_new_int][y_new_int] + dt_y * previous[x_new_int][y_new_int + 1]) +
                                dt_x * ((1 - dt_y) * previous[x_new_int + 1][y_new_int] + dt_y * previous[x_new_int + 1][y_new_int + 1]);
            }
        }
    }

    static void advect(Matrix& current, const Matrix& previous, const Matrix& u, const Matrix& v, float dt) {
        const float ratio = dt * static_cast<float>(g_cell_count - 1);
        std::thread partial_1(advect_thread, std::ref(current), std::ref(previous), std::ref(u), std::ref(v), ratio, 1, g_half_points, 1, g_half_points);
        std::thread partial_2(advect_thread, std::ref(current), std::ref(previous), std::ref(u), std::ref(v), ratio, g_half_points, g_cell_count - 1, 1, g_half_points);
        std::thread partial_3(
            advect_thread, std::ref(current), std::ref(previous), std::ref(u), std::ref(v), ratio, g_half_points, g_cell_count - 1, g_half_points, g_cell_count - 1);
        std::thread partial_4(advect_thread, std::ref(current), std::ref(previous), std::ref(u), std::ref(v), ratio, 1, g_half_points, g_half_points, g_cell_count - 1);
        partial_1.join();
        partial_2.join();
        partial_3.join();
        partial_4.join();
        set_bounds_to_zero(current);
    }

    static void add_scaled_thread(Matrix& matrix, const Matrix& addend, float weight, int start_i, int end_i, int start_j, int end_j) {
        for (int i = start_i; i != end_i; ++i) {
            for (int j = start_j; j != end_j; ++j) {
                matrix[i][j] += addend[i][j] * weight;
            }
        }
    }

    static void add_scaled(Matrix& matrix, const Matrix& addend, float weight) {
        std::thread partial_1(add_scaled_thread, std::ref(matrix), std::ref(addend), weight, 0, g_half_points, 0, g_half_points);
        std::thread partial_2(add_scaled_thread, std::ref(matrix), std::ref(addend), weight, g_half_points, g_cell_count - 1, 0, g_half_points);
        std::thread partial_3(add_scaled_thread, std::ref(matrix), std::ref(addend), weight, g_half_points, g_cell_count - 1, g_half_points, g_cell_count - 1);
        std::thread partial_4(add_scaled_thread, std::ref(matrix), std::ref(addend), weight, 0, g_half_points, g_half_points, g_cell_count - 1);
        partial_1.join();
        partial_2.join();
        partial_3.join();
        partial_4.join();
    }

    static float horizontal_difference(const Matrix& matrix, int i, int j) {
        return matrix[i][j + 1] - matrix[i][j - 1];
    }

    static float vertical_difference(const Matrix& source, int i, int j) {
        return source[i + 1][j] - source[i - 1][j];
    }

    static void project_thread_1(Matrix& v_previous, const Matrix& u_current, const Matrix& v_current, int start_i, int end_i, int start_j, int end_j) {
        for (int i = start_i; i != end_i; ++i) {
            for (int j = start_j; j != end_j; ++j) {
                v_previous[i][j] = -0.5f * g_cell_length * (vertical_difference(u_current, i, j) + horizontal_difference(v_current, i, j));
            }
        }
    }

    static void project_thread_2(Matrix& u_previous, const Matrix& v_previous, int start_i, int end_i, int start_j, int end_j) {
        for (int i = start_i; i != end_i; ++i) {
            for (int j = start_j; j != end_j; ++j) {
                u_previous[i][j] = (v_previous[i][j] + sum_neighbors(u_previous, i, j)) / 4;
            }
        }
    }

    static void project_step_1(Matrix& v_previous, const Matrix& u_current, const Matrix& v_current) {
        std::thread partial_1(project_thread_1, std::ref(v_previous), std::ref(u_current), std::ref(v_current), 1, g_half_points, 1, g_half_points);
        std::thread partial_2(project_thread_1, std::ref(v_previous), std::ref(u_current), std::ref(v_current), g_half_points, g_cell_count, 1, g_half_points);
        std::thread partial_3(project_thread_1, std::ref(v_previous), std::ref(u_current), std::ref(v_current), g_half_points, g_cell_count, g_half_points, g_cell_count);
        std::thread partial_4(project_thread_1, std::ref(v_previous), std::ref(u_current), std::ref(v_current), 1, g_half_points, g_half_points, g_cell_count);
        partial_1.join();
        partial_2.join();
        partial_3.join();
        partial_4.join();
    }

    static void project_step_2(Matrix& u_previous, const Matrix& v_previous) {
        for (int k = 0; k != 10; ++k) {
            std::thread partial_1(project_thread_2, std::ref(u_previous), std::ref(v_previous), 1, g_half_points, 1, g_half_points);
            std::thread partial_2(project_thread_2, std::ref(u_previous), std::ref(v_previous), g_half_points + 1, g_cell_count, 1, g_half_points);
            std::thread partial_3(project_thread_2, std::ref(u_previous), std::ref(v_previous), g_half_points + 1, g_cell_count, g_half_points + 1, g_cell_count);
            std::thread partial_4(project_thread_2, std::ref(u_previous), std::ref(v_previous), 1, g_half_points, g_half_points + 1, g_cell_count);
            partial_1.join();
            partial_2.join();
            partial_3.join();
            partial_4.join();
            for (int i = 1; i != g_cell_count; ++i) {
                u_previous[i][g_half_points] = (v_previous[i][g_half_points] + sum_neighbors(u_previous, i, g_half_points)) / 4;
                if (i != g_half_points) {
                    u_previous[g_half_points][i] = (v_previous[g_half_points][i] + sum_neighbors(u_previous, g_half_points, i)) / 4;
                }
            }
        }
    }

    static void project(Matrix& u_current, Matrix& v_current, Matrix& u_previous, Matrix& v_previous) {
        u_previous.set_zero();
        project_step_1(v_previous, u_current, v_current);
        project_step_2(u_previous, v_previous);
        for (int i = 1; i != g_cell_count; ++i) {
            for (int j = 1; j != g_cell_count; ++j) {
                u_current[i][j] -= 0.5f * vertical_difference(u_previous, i, j) / g_cell_length;
                v_current[i][j] -= 0.5f * horizontal_difference(u_previous, i, j) / g_cell_length;
            }
        }
        set_bounds_to_zero(u_current);
        set_bounds_to_zero(v_current);
    }

    static std::pair<int, int> screen_to_array_indices(float x, float y) {
        const int i = static_cast<int>(math::clamp(x, 0.0f, 0.999f) * g_point_count);
        const int j = static_cast<int>(math::clamp(y, 0.0f, 0.999f) * g_point_count);
        return {i, j};
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

    void Fluid::add_density(float x, float y, float multiplier) {
        if (x <= 0 || x >= 1 || y <= 0 || y >= 1) {
            return;
        }
        const auto [i_min, j_min]   = screen_to_array_indices(x - 0.03f, y - 0.03f);
        const auto [i_plus, j_plus] = screen_to_array_indices(x + 0.03f, y + 0.03f);

        for (int i = i_min; i != i_plus + 1; ++i) {
            for (int j = j_min; j != j_plus + 1; ++j) {
                m_density_previous[i][j] = multiplier * g_particle_input;
            }
        }
    }

    void Fluid::add_velocity(float x, float y, float dx, float dy) {
        if (x <= 0 || x >= 1 || y <= 0 || y >= 1) {
            return;
        }
        const auto [i, j]  = screen_to_array_indices(x, y);
        m_u_previous[i][j] = g_force_input * dx;
        m_v_previous[i][j] = g_force_input * dy;
    }

    void Fluid::clear_previous() {
        m_u_previous.set_zero();
        m_v_previous.set_zero();
        m_density_previous.set_zero();
    }

    void Fluid::clear_current() {
        m_u.set_zero();
        m_v.set_zero();
        m_density.set_zero();
    }

    void Fluid::density_step(float dt) {
        add_scaled(m_density, m_density_previous, dt);
        diffuse(m_density_previous, m_density, dt);
        advect(m_density, m_density_previous, m_u, m_v, dt);
    }

    void Fluid::velocity_step(float dt) {
        add_scaled(m_u, m_u_previous, dt);
        add_scaled(m_v, m_v_previous, dt);
        diffuse(m_u_previous, m_u, g_viscosity_coefficient);
        diffuse(m_v_previous, m_v, g_viscosity_coefficient);
        project(m_u_previous, m_v_previous, m_u, m_v);
        advect(m_u, m_u_previous, m_u_previous, m_v_previous, dt);
        advect(m_v, m_v_previous, m_u_previous, m_v_previous, dt);
        project(m_u, m_v, m_u_previous, m_v_previous);
    }

    void Fluid::step(float dt) {
        auto start = std::chrono::high_resolution_clock::now();

        {
            velocity_step(dt);
            auto stop     = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "Velocity step\t" << duration.count() << ",\t";
        }
        {
            density_step(dt);
            auto stop     = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "Density step\t" << duration.count() << "\n";
        }
        decrease_density();

        //        auto stop     = std::chrono::high_resolution_clock::now();
        //        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        //        std::cout << duration.count() << std::endl;
    }

    void Fluid::decrease_density() {
        for (int i = 0; i != g_cell_count; ++i) {
            for (int j = 0; j != g_cell_count; ++j) {
                m_density[i][j] *= 0.99;
            }
        }
    }

    void Fluid::add_bar() {
        for (size_t i = 0; i != 20; ++i) {
            float r             = static_cast<float>(rand()) / static_cast<float>(6.0f * RAND_MAX);
            auto [i_min, j_min] = screen_to_array_indices(static_cast<float>(i) / 20.0f, 0.0f);
            auto [i_max, j_max] = screen_to_array_indices(static_cast<float>(i + 1) / 2.0f, r);
            assert(i_min <= i_max);
            assert(j_min <= j_max);
            const float du = g_force_input * (static_cast<float>(rand()) / (92230.0 * RAND_MAX)) * ((rand() % 10) > 4 ? -1.0 : 1.0f);
            for (int w = i_min; w <= i_max; ++w) {
                for (int h = j_min; h <= j_max; ++h) {
                    m_density_previous[w][h] = 0.01 * g_particle_input;
                    m_v_previous[w][h]       = 0.0001 * g_force_input * r;
                    m_u_previous[w][h]       = du;
                }
            }
        }

        //        for (size_t i = 0; i != 20; ++i) {
        //            float r             = static_cast<float>(rand()) / static_cast<float>(6.0f * RAND_MAX);
        //            auto [i_min, j_min] = screen_to_array_indices(static_cast<float>(i) / 20.0f, 1.0f - r);
        //            auto [i_max, j_max] = screen_to_array_indices(static_cast<float>(i + 1) / 20.0f, 1.0f);
        //            assert(i_min <= i_max);
        //            assert(j_min <= j_max);
        //            const float du = g_force_input * (static_cast<float>(rand()) / (9230.0 * RAND_MAX)) * ((rand() % 10) > 4 ? -1.0 : 1.0f);
        //            for (int w = i_min; w <= i_max; ++w) {
        //                for (int h = j_min; h <= j_max; ++h) {
        //                    m_density_previous[w][h] = -1.0 * g_particle_input;
        //                    m_v_previous[w][h]       = -0.001 * g_force_input * r;
        //                    m_u_previous[w][h]       = du;
        //                }
        //            }
        //        }
    }

} // namespace app::fl