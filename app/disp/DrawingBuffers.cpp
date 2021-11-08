//
// Created by pc on 08-11-21.
//

#include "DrawingBuffers.h"

#include "../Math.h"
#include "../fl/Fluid.h"

#include <cmath>
#include <iostream>

namespace app::disp {

    DrawingBuffers::DrawingBuffers() : m_index_buffer(QOpenGLBuffer::IndexBuffer) {
    }

    DrawingBuffers::~DrawingBuffers() {
        m_array_buffer.destroy();
        m_index_buffer.destroy();
    }

    void DrawingBuffers::init() {
        init_buffers_and_vectors();
    }

    void DrawingBuffers::init_buffers_and_vectors() {
        m_array_buffer.create();
        m_index_buffer.create();

        m_vertices.resize(5 * m_horizontal_points * m_vertical_points);
        m_indices.resize(4 * (m_horizontal_points - 1) * (m_vertical_points - 1));

        for (size_t j = 0; j != m_vertical_points; ++j) {
            for (size_t i = 0; i != m_horizontal_points; ++i) {
                const size_t index    = 5 * (i + j * m_horizontal_points);
                m_vertices[index]     = i * 2.0f / static_cast<float>(m_horizontal_points - 1) - 1.0f;
                m_vertices[index + 1] = j * 2.0f / static_cast<float>(m_vertical_points - 1) - 1.0f;
            }
        }

        size_t index = 0;
        for (size_t i = 0; i != m_horizontal_points - 1; ++i) {
            for (size_t j = 0; j != m_vertical_points - 1; ++j) {
                size_t current_point = i + j * m_horizontal_points;
                m_indices[index]     = current_point;
                m_indices[index + 1] = current_point + m_horizontal_points;
                m_indices[index + 2] = current_point + 1 + m_horizontal_points;
                m_indices[index + 3] = current_point + 1;
                index += 4;
            }
        }
    }

    size_t DrawingBuffers::quad_count() const {
        return m_array_buffer.size() / 5;
    }

    static float project(float x) {
        x = math::clamp(x, -1.0f, 1.0f);
        if (x < 0.0f) {
            x *= -1;
        }
        return math::clamp(std::sqrt(std::sqrt(x)), 0.0f, 1.0f);
        //        return std::round(3.0f * std::sqrt(std::sqrt(x))) / 3.0f;
    }

    void DrawingBuffers::fill_and_bind(const app::fl::Fluid& m_fluid) {
        for (size_t j = 0; j != m_vertical_points; ++j) {
            for (size_t i = 0; i != m_horizontal_points; ++i) {
                const size_t index = 5 * (i + j * m_horizontal_points);
                //                if ((i + j) & 1) {
                const float r         = project(m_fluid.sample_density_at(i / static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 150.0f);
                const float g         = project(m_fluid.sample_u_at(i / static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 12.0f);
                const float b         = project(m_fluid.sample_v_at(i / static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 12.0f);
                m_vertices[index + 2] = 1.0f - r;
                m_vertices[index + 3] = 1.0f - r * g;
                m_vertices[index + 4] = 1.0f - r * b;
                //                } else {
                //                    const auto r          = project(m_fluid.sample_density_at(i / static_cast<float>(m_horizontal_points), j /
                //                    static_cast<float>(m_vertical_points)) / 150.0f); const auto g          = project(m_fluid.sample_u_at(i /
                //                    static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 1.0f); const auto b          =
                //                    project(m_fluid.sample_v_at(i / static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 1.0f); m_vertices[index
                //                    + 2] = r * b; m_vertices[index + 3] = r; m_vertices[index + 4] = r * g;
                //                }
            }
        }

        m_array_buffer.bind();
        m_array_buffer.allocate(m_vertices.data(), m_vertices.size() * sizeof(float));
        m_index_buffer.bind();
        m_index_buffer.allocate(m_indices.data(), m_indices.size() * sizeof(GLuint));
    }

} // namespace app::disp
