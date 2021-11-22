//
// Created by pc on 22-11-21.
//

#include "ShaderBase.h"

#include "DispSettings.h"

namespace app::disp {

    void ShaderBase::set_uniforms() {
    }

    void ShaderBase::update([[maybe_unused]] float dt) {
    }

    void ShaderBase::set_locations() {
        const int position_location = m_program.attributeLocation("position");
        m_program.enableAttributeArray(position_location);
        m_program.setAttributeBuffer(position_location, GL_FLOAT, 0, 2, 5 * sizeof(float));

        const int color_location = m_program.attributeLocation("color");
        m_program.enableAttributeArray(color_location);
        m_program.setAttributeBuffer(color_location, GL_FLOAT, 2 * sizeof(float), 3, 5 * sizeof(float));
    }

    void ShaderBase::zoom() {
        s_zoom = g_zoom_depth;
    }

    void ShaderBase::bind() {
        m_program.bind();
    }

    void ShaderBase::unbind() {
        m_program.release();
    }

    bool ShaderBase::init(const char* vertex_shader, const char* fragment_shader) {
        if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader)) {
            return false;
        }
        if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader)) {
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

    void ShaderBase::update_statics(float dt) {
        s_time += dt;
        s_zoom *= g_zoom_decay;
        if (s_zoom < 0.001) {
            s_zoom = 0.0f;
        }
    }
} // namespace app::disp