//
// Created by pc on 03-11-21.
//

#ifndef H_APP_FLUID_FLUIDSETTINGS_H
#define H_APP_FLUID_FLUIDSETTINGS_H

#include <cstddef>

namespace app::fluid {

    static constexpr size_t g_point_count = 140;
    static constexpr size_t g_cell_count  = g_point_count - 1;
    static constexpr float  g_cell_length = 1.0f / static_cast<float>(g_cell_count - 1);

    extern float g_particle_input;
    extern float g_force_input;
    extern float g_diffusion_coefficient;
    extern float g_viscosity_coefficient;
    extern float g_dt;

} // namespace app::fluid

#endif // H_APP_FLUID_FLUIDSETTINGS_H
