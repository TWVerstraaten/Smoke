//
// Created by pc on 08-11-21.
//

#include "SmokeShader.h"

#include "DispSettings.h"
#include "Shaders/SmokeFragmentShader.h"
#include "Shaders/SmokeVertexShader.h"

namespace app::disp {

    bool SmokeShader::init() {
        return ShaderBase::init(g_smoke_vertex_shader, g_smoke_fragment_shader);
    }

    void SmokeShader::set_uniforms() {
        m_program.setUniformValue("time", s_time);
        m_program.setUniformValue("zoom", 1.0f + s_zoom);
        m_program.setUniformValue("color_mode", static_cast<int>(g_color_mode));
        m_program.setUniformValue("invert_colors", g_invert_colors);
        m_program.setUniformValue("clamp_coefficient", g_clamp_coefficient);
        m_program.setUniformValue("clamp_count", static_cast<int>(g_clamp_count));
        m_program.setUniformValue("power_scale", g_power_scale);
    }

} // namespace app::disp
