#include "Fluid.h"

#include "../app/tools/Math.h"
#include "../app/tools/Profile.h"
#include "../app/tools/ThreadPool.h"
#include "../app/tools/ThreadSettings.h"

#include <cassert>
#include <cmath>

namespace app {
    namespace fluid {

        // See Jos Stam's paper "Real-Time Fluid Dynamics for Games" for the algorithm below

        static void                      set_bounds_to_zero(Matrix& matrix);
        static void                      diffuse_thread(Matrix& current, const Matrix& previous, float ratio, size_t start_j, size_t end_j);
        static void                      diffuse(Matrix& current, const Matrix& previous, float weight);
        static void                      advect_thread(Matrix& current, const Matrix& previous, const Matrix& u, const Matrix& v, float ratio, size_t start_j, size_t end_j);
        static void                      advect(Matrix& current, const Matrix& previous, const Matrix& u, const Matrix& v);
        static void                      project_thread_1(Matrix& v_previous, const Matrix& u_current, const Matrix& v_current, size_t start_j, size_t end_j);
        static void                      project_step_1(Matrix& v_previous, const Matrix& u_current, const Matrix& v_current);
        static void                      project_thread_2(Matrix& u_previous, size_t start_j, size_t end_j);
        static void                      project_step_2(Matrix& u_previous, const Matrix& v_previous);
        static void                      project_thread_3(Matrix& u_current, Matrix& v_current, const Matrix& u_previous, size_t start_j, size_t end_j);
        static void                      project_step_3(Matrix& u_current, Matrix& v_current, const Matrix& u_previous);
        static void                      project(Matrix& u_current, Matrix& v_current, Matrix& u_previous, Matrix& v_previous);
        static float                     clamp_to_grid(float value);
        static float                     sum_neighbors(const Matrix& matrix, size_t i, size_t j);
        static float                     horizontal_difference(const Matrix& matrix, size_t i, size_t j);
        static float                     vertical_difference(const Matrix& source, size_t i, size_t j);
        static std::pair<size_t, size_t> screen_to_array_indices(float x, float y);

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
            for (size_t i = i_min; i != i_plus + 1; ++i) {
                for (size_t j = j_min; j != j_plus + 1; ++j) {
                    m_density_previous[i][j] = 10 * multiplier * g_particle_input;
                }
            }
        }

        void Fluid::add_velocity(float x, float y, float dx, float dy) {
            if (x <= 0 || x >= 1 || y <= 0 || y >= 1) {
                return;
            }
            const auto [i, j]  = screen_to_array_indices(x, y);
            m_u_previous[i][j] = 10 * g_force_input * dx;
            m_v_previous[i][j] = 10 * g_force_input * dy;
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

        void Fluid::density_step() {
            m_density.add_scaled(m_density_previous, g_dt);
            diffuse(m_density_previous, m_density, g_dt);
            advect(m_density, m_density_previous, m_u, m_v);
        }

        void Fluid::velocity_step() {
            m_u.add_scaled(m_u_previous, g_dt);
            m_v.add_scaled(m_v_previous, g_dt);
            diffuse(m_u_previous, m_u, g_viscosity_coefficient);
            diffuse(m_v_previous, m_v, g_viscosity_coefficient);
            project(m_u_previous, m_v_previous, m_u, m_v);
            advect(m_u, m_u_previous, m_u_previous, m_v_previous);
            advect(m_v, m_v_previous, m_u_previous, m_v_previous);
            project(m_u, m_v, m_u_previous, m_v_previous);
        }

        void Fluid::step() {
            //        PROFILE_FUNCTION();
            velocity_step();
            density_step();
            decrease_density();
        }

        void Fluid::decrease_density() {
            for (int i = 0; i != g_point_count - 1; ++i) {
                for (int j = 0; j != g_point_count - 1; ++j) {
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
                const float du = g_force_input * (static_cast<float>(rand()) / (92230.0f * RAND_MAX)) * ((rand() % 10) > 4 ? -1.0f : 1.0f);
                for (size_t w = i_min; w <= i_max; ++w) {
                    for (size_t h = j_min; h <= j_max; ++h) {
                        m_density_previous[w][h] = 0.01f * g_particle_input;
                        m_v_previous[w][h]       = 0.0001f * g_force_input * r;
                        m_u_previous[w][h]       = du;
                    }
                }
            }
        }

        void Fluid::set_circle() {
            for (size_t i = 0; i != g_point_count - 1; ++i) {
                for (size_t j = 0; j != g_point_count - 1; ++j) {
                    if (std::sqrt(tools::math::square(i - g_cell_count / 2) + tools::math::square(j - g_cell_count / 2)) < g_cell_count / 4.0f &&
                        std::sqrt(tools::math::square(i - g_cell_count / 2) + tools::math::square(j - g_cell_count / 2)) > g_cell_count / 4.0f - 4) {
                        m_density[i][j] = 120;
                        m_u[i][j] *= 3;
                        m_v[i][j] *= 3;
                    }
                }
            }
        }

        /******************** Static function implementations ************************/
        static void set_bounds_to_zero(Matrix& matrix) {
            for (size_t i = 0; i != g_point_count; ++i) {
                matrix[0][i]                 = 0;
                matrix[g_point_count - 1][i] = 0;
                matrix[i][0]                 = 0;
                matrix[i][g_point_count - 1] = 0;
            }
        }

        static float clamp_to_grid(float value) {
            return tools::math::clamp(value, 0.5f, static_cast<float>(g_cell_count) - 0.5f);
        }

        static float sum_neighbors(const Matrix& matrix, size_t i, size_t j) {
            assert(i > 0 && j > 0);
            return matrix[i - 1][j] + matrix[i + 1][j] + matrix[i][j - 1] + matrix[i][j + 1];
        }

        static void diffuse_thread(Matrix& current, const Matrix& previous, float ratio, size_t start_j, size_t end_j) {
            for (size_t k = 0; k != 10; ++k) {
                for (size_t j = start_j; j != end_j; ++j) {
                    for (size_t i = 1; i != g_point_count - 1; ++i) {
                        current[i][j] = (previous[i][j] + ratio * sum_neighbors(current, i, j)) / (1 + (4 * ratio));
                    }
                }
            }
        }

        static void diffuse(Matrix& current, const Matrix& previous, float weight) {
            float ratio = weight * g_diffusion_coefficient * (g_cell_count - 1.0f) * (g_cell_count - 1.0f);
            if (app::tools::g_multi_thread) {
                for (size_t j = 0; j != tools::g_thread_count; ++j) {
                    app::tools::ThreadPool::get().add([&, ratio, j] {
                        size_t start_j = (j * (g_cell_count - 1)) / tools::g_thread_count + 1;
                        size_t end_j   = ((j + 1) * (g_cell_count - 1)) / tools::g_thread_count + 1;
                        return diffuse_thread(std::ref(current), std::ref(previous), ratio, start_j, end_j);
                    });
                }
                app::tools::ThreadPool::get().wait();
            } else {
                diffuse_thread(current, previous, ratio, 1, g_point_count - 1);
            }
            set_bounds_to_zero(current);
        }

        static void advect_thread(Matrix& current, const Matrix& previous, const Matrix& u, const Matrix& v, float ratio, size_t start_j, size_t end_j) {
            for (size_t j = start_j; j != end_j; ++j) {
                for (size_t i = 0; i != g_cell_count; ++i) {
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

        static void advect(Matrix& current, const Matrix& previous, const Matrix& u, const Matrix& v) {
            const float ratio = g_dt * static_cast<float>(g_cell_count - 1);
            if (app::tools::g_multi_thread) {
                for (size_t j = 0; j != tools::g_thread_count; ++j) {
                    app::tools::ThreadPool::get().add([&, ratio, j] {
                        size_t start_j = (j * (g_cell_count - 2)) / tools::g_thread_count + 1;
                        size_t end_j   = ((j + 1) * (g_cell_count - 1)) / tools::g_thread_count + 1;
                        return advect_thread(std::ref(current), std::ref(previous), std::ref(u), std::ref(v), ratio, start_j, end_j);
                    });
                }
                app::tools::ThreadPool::get().wait();
            } else {
                advect_thread(current, previous, u, v, ratio, 1, g_cell_count - 1);
            }
            set_bounds_to_zero(current);
        }

        static float horizontal_difference(const Matrix& matrix, size_t i, size_t j) {
            return matrix[i][j + 1] - matrix[i][j - 1];
        }

        static float vertical_difference(const Matrix& source, size_t i, size_t j) {
            return source[i + 1][j] - source[i - 1][j];
        }

        static void project_thread_1(Matrix& v_previous, const Matrix& u_current, const Matrix& v_current, size_t start_j, size_t end_j) {
            for (size_t j = start_j; j != end_j; ++j) {
                for (size_t i = 1; i != g_point_count - 1; ++i) {
                    v_previous[i][j] = -0.5f * g_cell_length * (vertical_difference(u_current, i, j) + horizontal_difference(v_current, i, j));
                }
            }
        }

        static void project_step_1(Matrix& v_previous, const Matrix& u_current, const Matrix& v_current) {
            if (app::tools::g_multi_thread) {
                for (size_t j = 0; j != tools::g_thread_count; ++j) {
                    app::tools::ThreadPool::get().add([&, j] {
                        size_t start_j = (j * (g_cell_count - 1)) / tools::g_thread_count + 1;
                        size_t end_j   = ((j + 1) * (g_cell_count - 1)) / tools::g_thread_count + 1;
                        return project_thread_1(v_previous, u_current, v_current, start_j, end_j);
                    });
                }
                app::tools::ThreadPool::get().wait();
            } else {
                project_thread_1(v_previous, u_current, v_current, 1, g_cell_count);
            }
        }

        static void project_thread_2(Matrix& u_previous, size_t start_j, size_t end_j) {
            for (size_t j = start_j; j != end_j; ++j) {
                for (size_t i = 1; i != g_point_count - 1; ++i) {
                    u_previous[i][j] = sum_neighbors(u_previous, i, j) / 4.0f;
                }
            }
        }

        static void project_step_2(Matrix& u_previous, const Matrix& v_previous) {
            if (app::tools::g_multi_thread) {
                for (size_t k = 0; k != 10; ++k) {
                    for (size_t j = 0; j != tools::g_thread_count; ++j) {
                        app::tools::ThreadPool::get().add([&, j] {
                            size_t start_j = (j * (g_cell_count - 1)) / tools::g_thread_count + 1;
                            size_t end_j   = ((j + 1) * (g_cell_count - 1)) / tools::g_thread_count + 1;
                            return project_thread_2(u_previous, start_j, end_j);
                        });
                    }
                    app::tools::ThreadPool::get().wait();
                    u_previous.add_scaled(v_previous, 0.25);
                }
            } else {
                for (size_t k = 0; k != 10; ++k) {
                    project_thread_2(u_previous, 1, g_point_count - 1);
                    u_previous.add_scaled(v_previous, 0.25);
                }
            }
        }

        static void project_thread_3(Matrix& u_current, Matrix& v_current, const Matrix& u_previous, size_t start_j, size_t end_j) {
            for (size_t j = start_j; j != end_j; ++j) {
                for (size_t i = 1; i != g_point_count - 1; ++i) {
                    u_current[i][j] -= 0.5f * vertical_difference(u_previous, i, j) / g_cell_length;
                    v_current[i][j] -= 0.5f * horizontal_difference(u_previous, i, j) / g_cell_length;
                }
            }
        }

        static void project_step_3(Matrix& u_current, Matrix& v_current, const Matrix& u_previous) {
            if (app::tools::g_multi_thread) {
                for (size_t j = 0; j != tools::g_thread_count; ++j) {
                    app::tools::ThreadPool::get().add([&, j] {
                        size_t start_j = (j * (g_cell_count - 2)) / tools::g_thread_count + 1;
                        size_t end_j   = ((j + 1) * (g_cell_count - 1)) / tools::g_thread_count + 1;
                        return project_thread_3(std::ref(u_current), std::ref(v_current), std::ref(u_previous), start_j, end_j);
                    });
                }
                app::tools::ThreadPool::get().wait();
            } else {
                project_thread_3(u_current, v_current, u_previous, 1, g_point_count - 1);
            }
        }

        static void project(Matrix& u_current, Matrix& v_current, Matrix& u_previous, Matrix& v_previous) {
            u_previous.set_zero();
            project_step_1(v_previous, u_current, v_current);
            project_step_2(u_previous, v_previous);
            project_step_3(u_current, v_current, u_previous);
            set_bounds_to_zero(u_current);
            set_bounds_to_zero(v_current);
        }

        static std::pair<size_t, size_t> screen_to_array_indices(float x, float y) {
            const auto i = static_cast<size_t>(tools::math::clamp(x, 0.0f, 1.0f) * (g_point_count - 1));
            const auto j = static_cast<size_t>(tools::math::clamp(y, 0.0f, 1.0f) * (g_point_count - 1));
            return {i, j};
        }

    } // namespace fluid
} // namespace app
