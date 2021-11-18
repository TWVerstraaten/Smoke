//
// Created by pc on 08-11-21.
//

#include "SmokeRenderer.h"

#include "../Math.h"
#include "../fl/Fluid.h"
#include "../tools/Profile.h"
#include "../tools/ThreadPool.h"
#include "../tools/ThreadSettings.h"
#include "DisplaySettings.h"
#include "ShaderSettings.h"

#include <cmath>

namespace app::disp {

    SmokeRenderer::SmokeRenderer() : m_quad_index_buffer(QOpenGLBuffer::IndexBuffer) {
    }

    SmokeRenderer::~SmokeRenderer() {
        m_quad_array_buffer.destroy();
        m_quad_index_buffer.destroy();
    }

    void SmokeRenderer::init() {
        init_buffers_and_vectors();
    }

    void SmokeRenderer::init_buffers_and_vectors() {
        m_quad_array_buffer.create();
        m_quad_index_buffer.create();

        m_quad_vertices.resize(5 * g_horizontal_sample_points * g_vertical_sample_points);
        m_quad_indices.resize(4 * (g_horizontal_sample_points - 1) * (g_vertical_sample_points - 1));

        for (size_t j = 0; j != g_vertical_sample_points; ++j) {
            for (size_t i = 0; i != g_horizontal_sample_points; ++i) {
                const size_t index         = 5 * (i + j * g_horizontal_sample_points);
                m_quad_vertices[index]     = static_cast<float>(i) * 2.0f / (g_horizontal_sample_points - 1) - 1.0f;
                m_quad_vertices[index + 1] = static_cast<float>(j) * 2.0f / (g_vertical_sample_points - 1) - 1.0f;
            }
        }

        size_t index = 0;
        for (size_t i = 0; i != g_horizontal_sample_points - 1; ++i) {
            for (size_t j = 0; j != g_vertical_sample_points - 1; ++j) {
                size_t current_point      = i + j * g_horizontal_sample_points;
                m_quad_indices[index]     = current_point;
                m_quad_indices[index + 1] = current_point + g_horizontal_sample_points;
                m_quad_indices[index + 2] = current_point + 1 + g_horizontal_sample_points;
                m_quad_indices[index + 3] = current_point + 1;
                index += 4;
            }
        }
    }

    size_t SmokeRenderer::quad_count() const {
        return m_quad_array_buffer.size() / 5;
    }

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

    void SmokeRenderer::fill_quads(const app::fl::Fluid& m_fluid) {
        PROFILE_FUNCTION();

        const auto fill_color_function = [&, this](size_t start, size_t end) {
            for (size_t j = start; j != end; ++j) {
                for (size_t i = 0; i != g_horizontal_sample_points; ++i) {
                    const size_t index = 5 * (i + j * g_horizontal_sample_points);
                    const float  r =
                        project(m_fluid.sample_density_at(i / static_cast<float>(g_horizontal_sample_points), j / static_cast<float>(g_vertical_sample_points)) / 150.0f);
                    const float g = project(m_fluid.sample_u_at(i / static_cast<float>(g_horizontal_sample_points), j / static_cast<float>(g_vertical_sample_points)) / 12.0f);
                    const float b = project(m_fluid.sample_v_at(i / static_cast<float>(g_horizontal_sample_points), j / static_cast<float>(g_vertical_sample_points)) / 12.0f);
                    m_quad_vertices[index + 2] = 1.0f - r;
                    m_quad_vertices[index + 3] = 1.0f - std::sqrt(r) * g;
                    m_quad_vertices[index + 4] = 1.0f - std::sqrt(r) * b;
                }
            }
        };

        if (tools::g_multi_thread) {
            for (size_t t = 0; t != tools::g_thread_count; ++t) {
                tools::ThreadPool::get().add([&, t] {
                    const size_t start = t * g_vertical_sample_points / tools::g_thread_count;
                    const size_t end   = (t + 1) * g_vertical_sample_points / tools::g_thread_count;
                    fill_color_function(start, end);
                });
            }
            tools::ThreadPool::get().wait();
        } else {
            fill_color_function(0, g_vertical_sample_points);
        }
    }

    void SmokeRenderer::bind_quads() {
        m_quad_array_buffer.bind();
        m_quad_array_buffer.allocate(m_quad_vertices.data(), m_quad_vertices.size() * sizeof(float));
        m_quad_index_buffer.bind();
        m_quad_index_buffer.allocate(m_quad_indices.data(), m_quad_indices.size() * sizeof(GLuint));
    }

    void SmokeRenderer::unbind() {
        m_quad_array_buffer.release();
        m_quad_index_buffer.release();
    }

} // namespace app::disp
