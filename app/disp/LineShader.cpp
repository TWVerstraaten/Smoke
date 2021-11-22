//
// Created by pc on 18-11-21.
//

#include "LineShader.h"

#include "Shaders/PlainFragmentShader.h"
#include "Shaders/PlainVertexShader.h"

namespace app::disp {

    bool LineShader::init() {
        return ShaderBase::init(g_plain_vertex_shader, g_plain_fragment_shader);
    }

    void LineShader::set_uniforms() {
        m_program.setUniformValue("zoom", 1.0f + s_zoom);
    }

} // namespace app::disp