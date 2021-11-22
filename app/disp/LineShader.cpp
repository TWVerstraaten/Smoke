//
// Created by pc on 18-11-21.
//

#include "LineShader.h"

#include "DisplaySettings.h"
#include "Shaders/LineFragmentShader.h"
#include "Shaders/LineVertexShader.h"

namespace app::disp {
    bool LineShader::init() {
        if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, g_line_vertex_shader)) {
            return false;
        }
        if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, g_line_fragment_shader)) {
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

    void LineShader::update([[maybe_unused]] float dt) {
        m_zoom *= g_zoom_decay;
        if (m_zoom < 0.0001) {
            m_zoom = 0.0f;
        }
    }

    void LineShader::set_locations() {
        const int position_location = m_program.attributeLocation("position");
        m_program.enableAttributeArray(position_location);
        m_program.setAttributeBuffer(position_location, GL_FLOAT, 0, 2, 5 * sizeof(float));

        const int color_location = m_program.attributeLocation("color");
        m_program.enableAttributeArray(color_location);
        m_program.setAttributeBuffer(color_location, GL_FLOAT, 2 * sizeof(float), 3, 5 * sizeof(float));
    }

    void LineShader::zoom() {
        m_zoom = g_zoom_depth;
    }

    void LineShader::unbind() {
        m_program.release();
    }

    void LineShader::bind() {
        m_program.bind();
    }

    void LineShader::set_uniforms() {
        m_program.setUniformValue("zoom", 1.0f + m_zoom);
    }

} // namespace app::disp