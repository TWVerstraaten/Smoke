//
// Created by pc on 01-11-21.
//

#ifndef H_APP_SHADER_H
#define H_APP_SHADER_H

#include <GL/glew.h>
#include <SFML/Graphics/Shader.hpp>

namespace app {

    class Shader {

      public:
        ~Shader();

        void init();

        void update_uniform(size_t elapsed_milliseconds) const;

      private:
        GLuint m_shader_program{};
        GLuint m_vertex_shader{};
        GLuint m_fragment_shader{};
        GLint  m_uniform_location = -1;
    };

} // namespace app

#endif // H_APP_SHADER_H
