//
// Created by pc on 22-11-21.
//

#include "DispSettings.h"

namespace app::disp {

    COLOR_MODE g_color_mode        = COLOR_MODE::WATER;
    bool       g_invert_colors     = false;
    float      g_zoom_depth        = 0.1;
    float      g_zoom_decay        = 0.987;
    float      g_clamp_coefficient = 0.2f;
    float      g_power_scale       = 0.7f;
    size_t     g_pixel_size        = 4;
    size_t     g_clamp_count       = 3;
    float      g_color_wash        = -0.2f;

} // namespace app::disp
