//
// Created by pc on 08-11-21.
//

#include "SmokeShader.h"

#include "ShaderSettings.h"
#include "Shaders/SmokeFragmentShader.h"
#include "Shaders/SmokeVertexShader.h"

#include <iostream>

namespace app::disp {

    bool SmokeShader::init() {
        if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, g_smoke_vertex_shader)) {
            return false;
        }
        if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, g_smoke_fragment_shader)) {
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

    void SmokeShader::update(float dt) {
        m_time += dt;
        m_offset += dt;
        m_zoom *= g_zoom_decay;
        if (m_zoom < 0.001) {
            m_zoom = 0.0f;
        }
        m_circle_timer -= dt;
        if (m_circle_timer <= 0) {
            m_circle_timer = 0.0f;
        }
    }

    void SmokeShader::set_locations() {
        const int position_location = m_program.attributeLocation("position");
        m_program.enableAttributeArray(position_location);
        m_program.setAttributeBuffer(position_location, GL_FLOAT, 0, 2, 5 * sizeof(float));

        const int color_location = m_program.attributeLocation("color");
        m_program.enableAttributeArray(color_location);
        m_program.setAttributeBuffer(color_location, GL_FLOAT, 2 * sizeof(float), 3, 5 * sizeof(float));
    }

    void SmokeShader::zoom() {
        m_zoom = g_zoom_depth;
    }

    void SmokeShader::set_circle() {
        m_circle_timer = 2.0f;
    }

    void SmokeShader::unbind() {
        m_program.release();
    }

    void SmokeShader::bind() {
        m_program.bind();
    }

    void SmokeShader::set_uniforms() {
        m_program.setUniformValue("time", m_time);
        m_program.setUniformValue("offset", m_offset);
        m_program.setUniformValue("zoom", 1.0f + m_zoom);
        m_program.setUniformValue("circle", m_circle_timer > 0.0f);
        m_program.setUniformValue("color_mode", g_color_mode);
        m_program.setUniformValue("invert_colors", g_invert_colors);
    }

} // namespace app::disp
