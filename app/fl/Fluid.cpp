#include "Fluid.h"

#include "../Math.h"

#include <cassert>
#include <cmath>

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

    static void diffuse(Matrix& current, Matrix& previous, float dt) {
        float ratio = dt * g_diffusion_coefficient * (g_cell_count - 1.0f) * (g_cell_count - 1.0f);
        for (int k = 0; k != 10; ++k) {
            for (int i = 1; i != g_cell_count; ++i) {
                for (int j = 1; j != g_cell_count; ++j) {
                    current[i][j] = (previous[i][j] + ratio * sum_neighbors(current, i, j)) / (1 + (4 * ratio));
                }
            }
            set_bounds_to_zero(current);
        }
    }

    static void advect(Matrix& current, const Matrix& previous, const Matrix& u, const Matrix& v, float dt) {
        float ratio = dt * static_cast<float>(g_cell_count - 1);
        for (int i = 1; i != g_cell_count; ++i) {
            for (int j = 1; j != g_cell_count; ++j) {
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
        set_bounds_to_zero(current);
    }

    static void add_scaled(Matrix& matrix, const Matrix& addend, float weight) {
        for (int i = 0; i != (g_cell_count + 1); ++i) {
            for (int j = 0; j != (g_cell_count + 1); ++j) {
                matrix[i][j] += addend[i][j] * weight;
            }
        }
    }

    static float horizontal_difference(Matrix& matrix, int i, int j) {
        return matrix[i][j + 1] - matrix[i][j - 1];
    }

    static float vertical_difference(Matrix& source, int i, int j) {
        return source[i + 1][j] - source[i - 1][j];
    }

    static void project(Matrix& u_current, Matrix& v_current, Matrix& u_previous, Matrix& v_previous) {
        float cell_length = 1.0f / static_cast<float>(g_cell_count - 1);
        for (int i = 1; i != g_cell_count; ++i) {
            for (int j = 1; j != g_cell_count; ++j) {
                v_previous[i][j] = -0.5f * cell_length * (vertical_difference(u_current, i, j) + horizontal_difference(v_current, i, j));
                u_previous[i][j] = 0;
            }
        }
        set_bounds_to_zero(v_previous);
        set_bounds_to_zero(u_previous);
        for (int k = 0; k != 10; ++k) {
            for (int i = 1; i != g_cell_count; ++i) {
                for (int j = 1; j != g_cell_count; ++j) {
                    u_previous[i][j] = (v_previous[i][j] + sum_neighbors(u_previous, i, j)) / 4;
                }
            }
            set_bounds_to_zero(u_previous);
        }
        for (int i = 1; i != g_cell_count; ++i) {
            for (int j = 1; j != g_cell_count; ++j) {
                u_current[i][j] -= 0.5f * vertical_difference(u_previous, i, j) / cell_length;
                v_current[i][j] -= 0.5f * horizontal_difference(u_previous, i, j) / cell_length;
            }
        }
        set_bounds_to_zero(u_current);
        set_bounds_to_zero(v_current);
    }

    static std::pair<int, int> screen_to_array_indices(float x, float y) {
        const int i = static_cast<int>(math::clamp(x, 0.0f, 0.99f) * g_point_count);
        const int j = static_cast<int>(math::clamp(y, 0.0f, 0.99f) * g_point_count);
        return {i, j};
    }

    float Fluid::sample_density_at(float x, float y) const {
        const auto [i, j] = screen_to_array_indices(x, y);
        return m_density[i][j];
    }

    float Fluid::sample_u_at(float x, float y) const {
        const auto [i, j] = screen_to_array_indices(x, y);
        return m_u[i][j];
    }

    float Fluid::sample_v_at(float x, float y) const {
        const auto [i, j] = screen_to_array_indices(x, y);
        return m_v[i][j];
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
        velocity_step(dt);
        density_step(dt);
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
                    m_density_previous[w][h] = 0.1 * g_particle_input;
                    m_v_previous[w][h]       = 0.001 * g_force_input * r;
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