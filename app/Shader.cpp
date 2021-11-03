//
// Created by pc on 01-11-21.
//

#include "Shader.h"

#include <cassert>
#include <iostream>

namespace app {

    static const GLchar* g_vertex_source = R"glsl(
    #version 430 core
    in vec2 position;
    in vec3 color;
    out vec3 Color;
    uniform float offset = 1.0;

    vec3 f(float r, float g, float b){
        return 0.5 * vec3(r,g,b) + 0.5 *vec3(1.0f - g, 1.0f - b, 1.0f - r);
    }

    vec3 f2(float r, float g, float b){
        float c =  0.5 + 0.5 * sin(offset);
        return c * vec3(r, g * sin(2.12 * offset) + r * cos(2.12 * offset), b) + (1.0f - c) * vec3(1.0f - g, 1.0f - b, 1.0f - r);
    }

    void main()
    {
        Color = f2(color[0],color[1],color[2]);
//        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

    static const GLchar* g_fragment_source = R"glsl(
    #version 430 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";

    void Shader::init() {
        m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);

        // Create and compile the vertex shader
        glShaderSource(m_vertex_shader, 1, &g_vertex_source, nullptr);
        glCompileShader(m_vertex_shader);

        // Create and compile the fragment shader
        m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_fragment_shader, 1, &g_fragment_source, nullptr);
        glCompileShader(m_fragment_shader);

        // Link the vertex and fragment shader into a shader program
        m_shader_program = glCreateProgram();
        glAttachShader(m_shader_program, m_vertex_shader);
        glAttachShader(m_shader_program, m_fragment_shader);
        glBindFragDataLocation(m_shader_program, 0, "outColor");
        glLinkProgram(m_shader_program);
        glUseProgram(m_shader_program);

        // Specify the layout of the vertex data
        GLint position_attrib = glGetAttribLocation(m_shader_program, "position");
        glEnableVertexAttribArray(position_attrib);
        glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

        GLint color_attrib = glGetAttribLocation(m_shader_program, "color");
        glEnableVertexAttribArray(color_attrib);
        glVertexAttribPointer(color_attrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

        m_uniform_location = glGetUniformLocation(m_shader_program, "offset");
    }

    Shader::~Shader() {
        glDeleteProgram(m_shader_program);
        glDeleteShader(m_fragment_shader);
        glDeleteShader(m_vertex_shader);
    }

    void Shader::update_uniform(size_t elapsed_milliseconds) const {
        glUniform1f(m_uniform_location, static_cast<float>(elapsed_milliseconds) / 2000.0f);
    }
} // namespace app
