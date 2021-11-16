//
// Created by pc on 11-11-21.
//

#include "ShaderSettings.h"

namespace app::disp {

    float      g_zoom_depth    = 0.1;
    float      g_zoom_decay    = 0.86;
    COLOR_MODE g_color_mode    = COLOR_MODE::GRAY;
    bool       g_invert_colors = true;
    bool       g_clamp_colors  = false;
    size_t     g_clamp_count   = 4;
} // namespace app::disp