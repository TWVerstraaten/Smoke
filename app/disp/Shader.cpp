//
// Created by pc on 08-11-21.
//

#include "Shader.h"

#include "ShaderSettings.h"

namespace app::disp {

    static const GLchar* g_vertex_source = R"glsl(
    #version 430 core
    in vec2 position;
    in vec3 color;

    out vec3 Color;

    uniform float offset = 0.0;
    uniform float zoom = 1.0;
    uniform int   color_mode = 0;
    uniform bool  invert_colors = false;

    vec3 f(float r, float g, float b){
        return 0.5 * vec3(r,g,b) + 0.5 *vec3(1.0f - g, 1.0f - b, 1.0f - r);
    }

    vec3 f2(float r, float g, float b){
        float c =  0.5 + 0.5 * sin(offset);
        return c * vec3(r, g, 1.0f - b) + (1.0f - c) * vec3(1.0f - g, b, 1.0f - r);
    }

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0 / zoom);
        if (invert_colors){
            Color = vec3(1.0f - color.r, 1.0f - color.g, 1.0f - color.b);
        } else {
            Color = color;
        }
        if (color_mode == 0){
            Color = Color.rrr;
        } else if (color_mode == 1){
            Color = Color;
        } else if (color_mode == 2){
            Color = f2(Color.r, Color.g, Color.b);
        }
    }
)glsl";

    static const GLchar* g_fragment_source = R"glsl(
    #version 430 core
    in vec3 Color;
    out vec4 outColor;

    uniform int  screen_width;
    uniform int  screen_height;
    uniform bool circle = false;

    bool in_circle(vec2 coord){
        float x = ((2.0f * coord.x) / screen_width) - 1.0f;
        float y = ((2.0f * coord.y) / screen_height) - 1.0f;
        float r = x * x + y * y;
        if (r > 0.4 && r < 0.5){
            return true;
        } else {
            return false;
        }
    }

    void main()
    {
        if (circle && in_circle(gl_FragCoord.xy)){
            outColor =  vec4(1.0f - Color.r, 1.0f - Color.g, 1.0f - Color.b, 1.0);
        } else {
            outColor = vec4(Color, 1.0);
        }
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
        return true;
    }

    void Shader::update(float dt) {
        m_offset += dt;
        m_zoom *= g_zoom_decay;
        if (m_zoom < 0.001) {
            m_zoom = 0.0f;
        }
        m_circle_timer -= dt;
        if (m_circle_timer <= 0) {
            m_circle_timer = 0.0f;
        }

        m_program.setUniformValue("offset", m_offset);
        m_program.setUniformValue("zoom", 1.0f + m_zoom);
        m_program.setUniformValue("circle", m_circle_timer > 0.0f);
        m_program.setUniformValue("color_mode", g_color_mode);
        m_program.setUniformValue("invert_colors", g_invert_colors);
    }

    void Shader::set_locations() {
        const int position_location = m_program.attributeLocation("position");
        m_program.enableAttributeArray(position_location);
        m_program.setAttributeBuffer(position_location, GL_FLOAT, 0, 2, 5 * sizeof(float));

        const int color_location = m_program.attributeLocation("color");
        m_program.enableAttributeArray(color_location);
        m_program.setAttributeBuffer(color_location, GL_FLOAT, 2 * sizeof(float), 3, 5 * sizeof(float));
    }

    void Shader::zoom() {
        m_zoom = g_zoom_depth;
    }

    void Shader::set_circle() {
        m_circle_timer = 2.0f;
    }

    void Shader::set_uniform_screen_size(int width, int height) {
        m_program.setUniformValue("screen_width", width);
        m_program.setUniformValue("screen_height", height);
    }

} // namespace app::disp
