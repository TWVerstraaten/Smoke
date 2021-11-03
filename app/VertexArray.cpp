//
// Created by pc on 01-11-21.
//

#include "VertexArray.h"
#include "Fluid.h"
#include <random>
#include <vector>

namespace app {

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &vertex_array_object);
        glGenBuffers(1, &vertex_buffer_object);
        glGenBuffers(1, &element_buffer_object);
        m_vertices.resize(5 * g_horizontal_points * g_vertical_points);
        m_indices.resize(6 * (g_horizontal_points - 1) * (g_vertical_points - 1));

        size_t index = 0;
        for (size_t j = 0; j != g_vertical_points; ++j) {
            for (size_t i = 0; i != g_horizontal_points; ++i) {
                m_vertices[index] = i * 2.0 / (g_horizontal_points - 1) - 1.0f;
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
                m_indices[index] = current_point;
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
        glDeleteBuffers(1, &element_buffer_object);
        glDeleteBuffers(1, &vertex_buffer_object);
        glDeleteVertexArrays(1, &vertex_array_object);
    }

    void VertexArray::bind() {

        glBindVertexArray(vertex_array_object);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size(), m_vertices.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_DYNAMIC_DRAW);

        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    }

    //    void VertexArray::set_color(const std::array<std::array<float, 100>, 100> &array) {
    //        size_t idx = 0;
    //        for (size_t j = 0; j != g_vertical_points; ++j) {
    //            for (size_t i = 0; i != g_horizontal_points; ++i) {
    //                const size_t index = 5 * (i + j * g_horizontal_points);
    //                m_vertices[index + 2] = array[i][j] / 10;
    //                m_vertices[index + 3] = array[i][j] / 10;
    //                m_vertices[index + 4] = array[i][j] / 10;
    //            }
    //        }
    //        bind();
    //    }
}// namespace app