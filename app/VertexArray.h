//
// Created by pc on 01-11-21.
//

#ifndef H_APP_VERTEXARRAY_H
#define H_APP_VERTEXARRAY_H


#include <GL/glew.h>
#include <array>
#include <vector>

namespace app {

    static const size_t g_horizontal_points = 100;
    static const size_t g_vertical_points   = 100;

    class VertexArray {

      public:
        VertexArray();
        ~VertexArray();

        void bind();

//        void set_color(const std::array<std::array<float, 100>, 100> &array);

      private:
        GLuint vertex_array_object{};
        GLuint vertex_buffer_object{};
        GLuint element_buffer_object{};
        std::vector<GLfloat> m_vertices{};
        std::vector<GLuint> m_indices{};
    };

}// namespace app


#endif//H_APP_VERTEXARRAY_H
