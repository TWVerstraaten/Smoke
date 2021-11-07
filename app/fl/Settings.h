//
// Created by pc on 03-11-21.
//

#ifndef H_APP_FL_SETTINGS_H
#define H_APP_FL_SETTINGS_H

#include <cstddef>

namespace app::fl {

    static constexpr size_t g_point_count           = 200;
    static constexpr size_t g_cell_count            = g_point_count - 1;
    static constexpr float  g_particle_input        = 6800.0f;
    static constexpr float  g_force_input           = 2000000.0f;
    static constexpr float  g_diffusion_coefficient = 0.0003f;
    static constexpr float  g_viscosity_coefficient = 0.0004f;

} // namespace app

#endif // H_APP_FL_SETTINGS_H
