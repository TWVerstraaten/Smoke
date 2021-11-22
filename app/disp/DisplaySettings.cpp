//
// Created by pc on 22-11-21.
//

#include "DisplaySettings.h"

namespace app::disp {

    PIXEL_MODE g_pixel_mode    = PIXEL_MODE::PIXEL;
    COLOR_MODE g_color_mode    = COLOR_MODE::RGB;
    bool       g_invert_colors = false;
    bool       g_clamp_colors  = true;
    float      g_zoom_depth    = 0.1;
    float      g_zoom_decay    = 0.86;
    size_t     g_pixel_size    = 6;
    size_t     g_clamp_count   = 3;

} // namespace app::disp
