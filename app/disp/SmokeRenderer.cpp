//
// Created by pc on 08-11-21.
//

#include "SmokeRenderer.h"

#include "../../fluid/Fluid.h"
#include "../tools/Profile.h"
#include "../tools/ThreadPool.h"
#include "../tools/ThreadSettings.h"
#include "DispSettings.h"
#include "SmokeShader.h"

#include <cmath>

namespace app::disp {

    static float f_x(const size_t i, const size_t horizontal_sample_points) {
        return static_cast<float>(2 * i) / static_cast<float>(horizontal_sample_points - 1) - 1.0f;
    }

    static float f_y(const size_t j, const size_t vertical_sample_points) {
        return static_cast<float>(j) * 2.0f / static_cast<float>(vertical_sample_points - 1) - 1.0f;
    }

    static float f_r(const size_t i, const size_t j, const size_t horizontal_sample_points, const size_t vertical_sample_points, const fluid::Fluid& fluid) {
        return fluid.sample_density_at(static_cast<float>(i) / static_cast<float>(horizontal_sample_points), static_cast<float>(j) / static_cast<float>(vertical_sample_points)) /
               120.0f;
    }

    static float f_g(const size_t i, const size_t j, const size_t horizontal_sample_points, const size_t vertical_sample_points, const fluid::Fluid& fluid) {
        return fluid.sample_u_at(static_cast<float>(i) / static_cast<float>(horizontal_sample_points), static_cast<float>(j) / static_cast<float>(vertical_sample_points)) / 7.0f;
    }

    static float f_b(const size_t i, const size_t j, const size_t horizontal_sample_points, const size_t vertical_sample_points, const fluid::Fluid& fluid) {
        return fluid.sample_v_at(static_cast<float>(i) / static_cast<float>(horizontal_sample_points), static_cast<float>(j) / static_cast<float>(vertical_sample_points)) / 7.0f;
    }

    void SmokeRenderer::fill_quad_normal_thread(size_t start, size_t end, const fluid::Fluid& fluid) {
        for (size_t j = start; j != end; ++j) {
            for (size_t i = 0; i != m_horizontal_sample_points; ++i) {
                const size_t index         = 5 * (i + j * m_horizontal_sample_points);
                m_quad_vertices[index]     = f_x(i, m_horizontal_sample_points);
                m_quad_vertices[index + 1] = f_y(j, m_vertical_sample_points);
                m_quad_vertices[index + 2] = f_r(i, j, m_horizontal_sample_points, m_vertical_sample_points, fluid);
                m_quad_vertices[index + 3] = f_g(i, j, m_horizontal_sample_points, m_vertical_sample_points, fluid);
                m_quad_vertices[index + 4] = f_b(i, j, m_horizontal_sample_points, m_vertical_sample_points, fluid);
                if (i < m_horizontal_sample_points - 1 && j < m_vertical_sample_points - 1) {
                    const size_t current_point                                       = i + j * m_horizontal_sample_points;
                    m_quad_indices[4 * (i * (m_vertical_sample_points - 1) + j)]     = current_point;
                    m_quad_indices[4 * (i * (m_vertical_sample_points - 1) + j) + 1] = current_point + m_horizontal_sample_points;
                    m_quad_indices[4 * (i * (m_vertical_sample_points - 1) + j) + 2] = current_point + 1 + m_horizontal_sample_points;
                    m_quad_indices[4 * (i * (m_vertical_sample_points - 1) + j) + 3] = current_point + 1;
                }
            }
        }
    }

    void SmokeRenderer::fill_quad_pixel_thread(size_t start, size_t end, const fluid::Fluid& fluid) {
        for (size_t i = 0; i != m_horizontal_sample_points; ++i) {
            for (size_t j = start; j != end; ++j) {
                const size_t index         = 5 * (i * m_vertical_sample_points + j);
                m_quad_vertices[index]     = f_x(i, m_horizontal_sample_points);
                m_quad_vertices[index + 1] = f_y(j, m_vertical_sample_points);
                m_quad_vertices[index + 2] = f_r(i, j, m_horizontal_sample_points, m_vertical_sample_points, fluid);
                m_quad_vertices[index + 3] = f_g(i, j, m_horizontal_sample_points, m_vertical_sample_points, fluid);
                m_quad_vertices[index + 4] = f_b(i, j, m_horizontal_sample_points, m_vertical_sample_points, fluid);
                m_quad_indices[index / 5]  = index / 5;
            }
        }
    }

    SmokeRenderer::SmokeRenderer() {
        m_shader = std::unique_ptr<ShaderBase>(new SmokeShader);
    }

    void SmokeRenderer::fill(const fluid::Fluid& fluid) {
        if (g_pixel_mode == PIXEL_MODE::NORMAL) {
            fill_quads_normal(fluid);
        } else {
            fill_quads_pixel(fluid);
        }
    }

    void SmokeRenderer::fill_quads_normal(const fluid::Fluid& fluid) {
        m_quad_vertices.resize(5 * m_horizontal_sample_points * m_vertical_sample_points);
        m_quad_indices.resize(4 * (m_horizontal_sample_points - 1) * (m_vertical_sample_points - 1));
        if (not tools::g_multi_thread) {
            fill_quad_normal_thread(0, m_vertical_sample_points, fluid);
        } else {
            for (size_t t = 0; t != tools::g_thread_count; ++t) {
                tools::ThreadPool::get().add(
                    [&, t] { fill_quad_normal_thread(t * m_vertical_sample_points / tools::g_thread_count, (t + 1) * m_vertical_sample_points / tools::g_thread_count, fluid); });
            }
            tools::ThreadPool::get().wait();
        }
    }

    void SmokeRenderer::fill_quads_pixel(const fluid::Fluid& fluid) {
        m_quad_vertices.resize(5 * m_horizontal_sample_points * m_vertical_sample_points);
        m_quad_indices.resize(m_horizontal_sample_points * m_vertical_sample_points);
        if (not tools::g_multi_thread) {
            fill_quad_pixel_thread(0, m_vertical_sample_points, fluid);
        } else {
            for (size_t t = 0; t != tools::g_thread_count; ++t) {
                tools::ThreadPool::get().add(
                    [&, t] { fill_quad_pixel_thread(t * m_vertical_sample_points / tools::g_thread_count, (t + 1) * m_vertical_sample_points / tools::g_thread_count, fluid); });
            }
            tools::ThreadPool::get().wait();
        }
    }

    void SmokeRenderer::set_sample_points(size_t width, size_t height) {
        m_horizontal_sample_points = (width + 2 * g_pixel_size) / std::max(1ul, g_pixel_size - 1);
        m_vertical_sample_points   = (height + 2 * g_pixel_size) / std::max(1ul, g_pixel_size - 1);
    }

} // namespace app::disp
