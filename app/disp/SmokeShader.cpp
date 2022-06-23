//
// Created by pc on 08-11-21.
//

#include "SmokeShader.h"

#include "../../fluid/Fluid.h"
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
        m_program.setUniformValue("color_wash", g_color_wash);
        m_program.setUniformValue("max_density", m_max_density);
        m_program.setUniformValue("min_density", m_min_density);
        m_program.setUniformValue("max_u", m_max_u);
        m_program.setUniformValue("min_u", m_min_u);
        m_program.setUniformValue("max_v", m_max_v);
        m_program.setUniformValue("min_v", m_min_v);
        m_program.setUniformValue("width", static_cast<int>(m_width));
        m_program.setUniformValue("height", static_cast<int>(m_height));
    }

    void SmokeShader::set_extreme_values(const fluid::Fluid& fluid) {
        const auto& density = fluid.density();
        const auto& u       = fluid.u();
        const auto& v       = fluid.v();
        m_max_density       = density.max();
        m_min_density       = density.min();
        m_max_u             = u.max();
        m_min_u             = u.min();
        m_max_v             = v.max();
        m_min_v             = v.min();
    }

    void SmokeShader::resize(size_t width, size_t height) {
        m_width  = width;
        m_height = height;
    }

} // namespace app::disp
