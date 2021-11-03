//
// Created by pc on 01-11-21.
//

#ifndef H_APP_VERTEXARRAY_H
#define H_APP_VERTEXARRAY_H

#include <GL/glew.h>
#include <array>
#include <vector>

namespace app {

    static const size_t g_horizontal_points = 600;
    static const size_t g_vertical_points   = 600;

    class Fluid;

    class VertexArray {

      public:
        VertexArray();
        ~VertexArray();

        void bind();

        void set_color(const Fluid& fluid);

      private:
        GLuint               m_vertex_array_object{};
        GLuint               m_vertex_buffer_object{};
        GLuint               m_element_buffer_object{};
        std::vector<GLfloat> m_vertices{};
        std::vector<GLuint>  m_indices{};
    };

} // namespace app

#endif // H_APP_VERTEXARRAY_H
