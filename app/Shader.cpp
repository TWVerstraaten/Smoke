//
// Created by pc on 01-11-21.
//

#include "Shader.h"
#include <cassert>
#include <iostream>

namespace app {

    static const GLchar *vertexSource = R"glsl(
    #version 430 core
    in vec2 position;
    in vec3 color;
    out vec3 Color;
    uniform float offset = 1.0;

    vec3 f(float c, float x, float y){
        return vec3((1.0f - c) * (0.5 + 0.5 * sin(offset)), abs(sin(x * 1.74)) * (1.0f - c) , abs(sin(y * 1.09 + 0.21 * x)) * c);
    }

    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

    static const GLchar *fragmentSource = R"glsl(
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
        glShaderSource(m_vertex_shader, 1, &vertexSource, nullptr);
        glCompileShader(m_vertex_shader);

        // Create and compile the fragment shader
        m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_fragment_shader, 1, &fragmentSource, nullptr);
        glCompileShader(m_fragment_shader);


        // Link the vertex and fragment shader into a shader program
        m_shader_program = glCreateProgram();
        glAttachShader(m_shader_program, m_vertex_shader);
        glAttachShader(m_shader_program, m_fragment_shader);
        glBindFragDataLocation(m_shader_program, 0, "outColor");
        glLinkProgram(m_shader_program);
        glUseProgram(m_shader_program);

        // Specify the layout of the vertex data
        GLint posAttrib = glGetAttribLocation(m_shader_program, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

        GLint colAttrib = glGetAttribLocation(m_shader_program, "color");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));
    }

    Shader::~Shader() {
        glDeleteProgram(m_shader_program);
        glDeleteShader(m_fragment_shader);
        glDeleteShader(m_vertex_shader);
    }

    void Shader::update_uniform() {
        ++m_uniform_value;
        glUniform1f(glGetUniformLocation(m_shader_program, "offset"), m_uniform_value / static_cast<float>(120));
    }
}// namespace app
