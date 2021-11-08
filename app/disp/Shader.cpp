//
// Created by pc on 08-11-21.
//

#include "Shader.h"

namespace app::disp {

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
        return c * vec3(r, g, 1.0f - b) + (1.0f - c) * vec3(1.0f - g, b, 1.0f - r);
    }

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0) ;
        Color = color;
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

    bool Shader::init() {
        if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, g_vertex_source)) {
            return false;
        }
        if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, g_fragment_source)) {
            return false;
        }
        if (!m_program.link()) {
            return false;
        }
        if (!m_program.bind()) {
            return false;
        }
        m_offset_uniform = m_program.uniformLocation("offset");
        return true;
    }

    void Shader::update(float dt) {
        m_offset += dt;
        m_program.setUniformValue(m_offset_uniform, m_offset);
    }

    void Shader::set_locations() {
        const int position_location = m_program.attributeLocation("position");
        m_program.enableAttributeArray(position_location);
        m_program.setAttributeBuffer(position_location, GL_FLOAT, 0, 2, 5 * sizeof(float));

        const int color_location = m_program.attributeLocation("color");
        m_program.enableAttributeArray(color_location);
        m_program.setAttributeBuffer(color_location, GL_FLOAT, 2 * sizeof(float), 3, 5 * sizeof(float));
    }

} // namespace app::disp
