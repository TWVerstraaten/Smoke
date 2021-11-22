//
// Created by pc on 11-11-21.
//

#include "ShaderSettings.h"

namespace app::disp {

    float      g_zoom_depth    = 0.1;
    float      g_zoom_decay    = 0.86;
    bool       g_invert_colors = false;
    bool       g_clamp_colors  = true;
    size_t     g_clamp_count   = 3;
    COLOR_MODE g_color_mode    = COLOR_MODE::RGB;

} // namespace app::disp
