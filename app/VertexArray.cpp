//
// Created by pc on 01-11-21.
//

#include "VertexArray.h"

#include "Fluid.h"
#include "Math.h"

#include <random>
#include <vector>

namespace app {

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_vertex_array_object);
        glGenBuffers(1, &m_vertex_buffer_object);
        glGenBuffers(1, &m_element_buffer_object);
        m_vertices.resize(5 * g_horizontal_points * g_vertical_points);
        m_indices.resize(6 * (g_horizontal_points - 1) * (g_vertical_points - 1));

        size_t index = 0;
        for (size_t j = 0; j != g_vertical_points; ++j) {
            for (size_t i = 0; i != g_horizontal_points; ++i) {
                m_vertices[index]     = i * 2.0 / (g_horizontal_points - 1) - 1.0f;
                m_vertices[index + 1] = j * 2.0 / (g_vertical_points - 1) - 1.0f;
                m_vertices[index + 2] = i * 1.0 / (g_horizontal_points - 1);
                m_vertices[index + 3] = .5;
                m_vertices[index + 4] = rand() / static_cast<double>(RAND_MAX);
                index += 5;
            }
        }
        index = 0;
        for (size_t i = 0; i != g_horizontal_points - 1; ++i) {
            for (size_t j = 0; j != g_vertical_points - 1; ++j) {
                size_t current_point = i + j * g_horizontal_points;
                m_indices[index]     = current_point;
                m_indices[index + 1] = current_point + g_horizontal_points;
                m_indices[index + 2] = current_point + 1;
                m_indices[index + 3] = current_point + g_horizontal_points;
                m_indices[index + 4] = current_point + 1;
                m_indices[index + 5] = current_point + 1 + g_horizontal_points;
                index += 6;
            }
        }
    }

    VertexArray::~VertexArray() {
        glDeleteBuffers(1, &m_element_buffer_object);
        glDeleteBuffers(1, &m_vertex_buffer_object);
        glDeleteVertexArrays(1, &m_vertex_array_object);
    }

    void VertexArray::bind() {

        glBindVertexArray(m_vertex_array_object);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size(), m_vertices.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_object);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_DYNAMIC_DRAW);

        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    }

    float project(float x) {
        x = math::clamp(x, -1.0f, 1.0f);
        if (x < 0.01f) {
            x = 0.5f * (1.0f - x) + 0.2f;
        }
//        return math::clamp(std::sqrt(x), 0.0f, 1.0f);
                return (std::round(7.0f * std::sqrt(x))) / 7.0f;
    }

    void VertexArray::set_color(const Fluid& fluid) {
        for (size_t j = 0; j != g_vertical_points; ++j) {
            for (size_t i = 0; i != g_horizontal_points; ++i) {
                const size_t index    = 5 * (i + j * g_horizontal_points);
                const auto   r        = project(fluid.sample_density_at(i / static_cast<float>(g_horizontal_points), j / static_cast<float>(g_vertical_points)) / 150.0f);
                const auto   g        = project(fluid.sample_u_at(i / static_cast<float>(g_horizontal_points), j / static_cast<float>(g_vertical_points)) / 1.0f);
                const auto   b        = project(fluid.sample_v_at(i / static_cast<float>(g_horizontal_points), j / static_cast<float>(g_vertical_points)) / 1.0f);
                m_vertices[index + 2] = r;
                m_vertices[index + 3] = r * g;
                m_vertices[index + 4] = r * b;
            }
        }
        bind();
    }
} // namespace app
