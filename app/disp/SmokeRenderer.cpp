//
// Created by pc on 08-11-21.
//

#include "SmokeRenderer.h"

#include "../Math.h"
#include "../fl/Fluid.h"
#include "../tools/Profile.h"
#include "../tools/ThreadPool.h"
#include "../tools/ThreadSettings.h"
#include "DispSettings.h"

#include <cmath>

namespace app::disp {

    static float project(float x) {
        x = std::abs(x);
        x = math::clamp(x, 0.0f, 1.0f);
        //        if (x < 0.0f) {
        //            x *= -1;
        //        }
        if (g_clamp_colors) {
            return std::floor(static_cast<float>(g_clamp_count) * std::sqrt(std::sqrt(x))) / static_cast<float>(g_clamp_count - 1);
        } else {
            return math::clamp(std::sqrt(std::sqrt(x)), 0.0f, 1.0f);
        }
    }

    static float f_x(const size_t i, const size_t horizontal_sample_points) {
        return static_cast<float>(2 * i) / static_cast<float>(horizontal_sample_points - 1) - 1.0f;
    }

    static float f_y(const size_t j, const size_t vertical_sample_points) {
        return static_cast<float>(j) * 2.0f / static_cast<float>(vertical_sample_points - 1) - 1.0f;
    }

    static float f_r(const size_t i, const size_t j, const size_t horizontal_sample_points, const size_t vertical_sample_points, const fl::Fluid& fluid) {
        return project(
            fluid.sample_density_at(static_cast<float>(i) / static_cast<float>(horizontal_sample_points), static_cast<float>(j) / static_cast<float>(vertical_sample_points)) /
            150.0f);
    }

    static float f_g(const size_t i, const size_t j, const size_t horizontal_sample_points, const size_t vertical_sample_points, const fl::Fluid& fluid) {
        return project(fluid.sample_u_at(static_cast<float>(i) / static_cast<float>(horizontal_sample_points), static_cast<float>(j) / static_cast<float>(vertical_sample_points)) /
                       12.0f);
    }

    static float f_b(const size_t i, const size_t j, const size_t horizontal_sample_points, const size_t vertical_sample_points, const fl::Fluid& fluid) {
        return project(fluid.sample_v_at(static_cast<float>(i) / static_cast<float>(horizontal_sample_points), static_cast<float>(j) / static_cast<float>(vertical_sample_points)) /
                       12.0f);
    }

    static void fill_quad_normal_thread(size_t               start,
                                        size_t               end,
                                        size_t               horizontal_sample_points,
                                        size_t               vertical_sample_points,
                                        std::vector<float>&  quad_vertices,
                                        std::vector<GLuint>& quad_indices,
                                        const fl::Fluid&     fluid) {
        for (size_t j = start; j != end; ++j) {
            for (size_t i = 0; i != horizontal_sample_points; ++i) {
                const size_t index       = 5 * (i + j * horizontal_sample_points);
                quad_vertices[index]     = f_x(i, horizontal_sample_points);
                quad_vertices[index + 1] = f_y(j, vertical_sample_points);
                quad_vertices[index + 2] = f_r(i, j, horizontal_sample_points, vertical_sample_points, fluid);
                quad_vertices[index + 3] = f_g(i, j, horizontal_sample_points, vertical_sample_points, fluid);
                quad_vertices[index + 4] = f_b(i, j, horizontal_sample_points, vertical_sample_points, fluid);
                if (i < horizontal_sample_points - 1 && j < vertical_sample_points - 1) {
                    const size_t current_point                                   = i + j * horizontal_sample_points;
                    quad_indices[4 * (i * (vertical_sample_points - 1) + j)]     = current_point;
                    quad_indices[4 * (i * (vertical_sample_points - 1) + j) + 1] = current_point + horizontal_sample_points;
                    quad_indices[4 * (i * (vertical_sample_points - 1) + j) + 2] = current_point + 1 + horizontal_sample_points;
                    quad_indices[4 * (i * (vertical_sample_points - 1) + j) + 3] = current_point + 1;
                }
            }
        }
    }

    static void fill_quad_pixel_thread(size_t               start,
                                       size_t               end,
                                       size_t               horizontal_sample_points,
                                       size_t               vertical_sample_points,
                                       std::vector<float>&  quad_vertices,
                                       std::vector<GLuint>& quad_indices,
                                       const fl::Fluid&     fluid) {
        for (size_t j = start; j != end; ++j) {
            for (size_t i = 0; i != horizontal_sample_points; ++i) {
                size_t index             = 5 * (i * vertical_sample_points + j);
                quad_vertices[index]     = f_x(i, horizontal_sample_points);
                quad_vertices[index + 1] = f_y(j, vertical_sample_points);
                quad_vertices[index + 2] = f_r(i, j, horizontal_sample_points, vertical_sample_points, fluid);
                quad_vertices[index + 3] = f_g(i, j, horizontal_sample_points, vertical_sample_points, fluid);
                quad_vertices[index + 4] = f_b(i, j, horizontal_sample_points, vertical_sample_points, fluid);
                quad_indices[index / 5]  = index / 5;
            }
        }
    }

    void SmokeRenderer::fill(const fl::Fluid& fluid) {
        PROFILE_FUNCTION();
        if (g_pixel_mode == PIXEL_MODE::NORMAL) {
            fill_quads_normal(fluid);
        } else {
            fill_quads_pixel(fluid);
        }
    }

    void SmokeRenderer::fill_quads_normal(const fl::Fluid& fluid) {
        m_quad_vertices.resize(5 * m_horizontal_sample_points * m_vertical_sample_points);
        m_quad_indices.resize(4 * (m_horizontal_sample_points - 1) * (m_vertical_sample_points - 1));
        if (not tools::g_multi_thread) {
            fill_quad_normal_thread(0, m_vertical_sample_points, m_horizontal_sample_points, m_vertical_sample_points, m_quad_vertices, m_quad_indices, fluid);
        } else {
            for (size_t t = 0; t != tools::g_thread_count; ++t) {
                tools::ThreadPool::get().add([&, t] {
                    fill_quad_normal_thread(t * m_vertical_sample_points / tools::g_thread_count,
                                            (t + 1) * m_vertical_sample_points / tools::g_thread_count,
                                            m_horizontal_sample_points,
                                            m_vertical_sample_points,
                                            m_quad_vertices,
                                            m_quad_indices,
                                            fluid);
                });
            }
            tools::ThreadPool::get().wait();
        }
    }

    void SmokeRenderer::fill_quads_pixel(const fl::Fluid& fluid) {
        m_quad_vertices.resize(5 * m_horizontal_sample_points * m_vertical_sample_points);
        m_quad_indices.resize(m_horizontal_sample_points * m_vertical_sample_points);
        if (not tools::g_multi_thread) {
            fill_quad_pixel_thread(0, m_vertical_sample_points, m_horizontal_sample_points, m_vertical_sample_points, m_quad_vertices, m_quad_indices, fluid);
        } else {
            for (size_t t = 0; t != tools::g_thread_count; ++t) {
                tools::ThreadPool::get().add([&, t] {
                    fill_quad_pixel_thread(t * m_vertical_sample_points / tools::g_thread_count,
                                           (t + 1) * m_vertical_sample_points / tools::g_thread_count,
                                           m_horizontal_sample_points,
                                           m_vertical_sample_points,
                                           m_quad_vertices,
                                           m_quad_indices,
                                           fluid);
                });
            }
            tools::ThreadPool::get().wait();
        }
    }


    void SmokeRenderer::set_sample_points(size_t width, size_t height) {
        m_horizontal_sample_points = (width + 2 * g_pixel_size) / std::max(1ul, g_pixel_size - 1);
        m_vertical_sample_points   = (height + 2 * g_pixel_size) / std::max(1ul, g_pixel_size - 1);
    }

} // namespace app::disp
