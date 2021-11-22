//
// Created by pc on 17-11-21.
//

#ifndef H_APP_DISP_DISPSETTINGS_H
#define H_APP_DISP_DISPSETTINGS_H

#include <cstddef>

namespace app::disp {
    enum class PIXEL_MODE { NORMAL, PIXEL };
    enum class COLOR_MODE : int { GRAY = 0, RGB = 1, SPACE = 2 };

    extern PIXEL_MODE g_pixel_mode;
    extern COLOR_MODE g_color_mode;
    extern bool       g_invert_colors;
    extern float      g_zoom_depth;
    extern float      g_zoom_decay;
    extern float      g_clamp_coefficient;
    extern float      g_power_scale;
    extern size_t     g_pixel_size;
    extern size_t     g_clamp_count;

} // namespace app::disp

#endif // H_APP_DISP_DISPSETTINGS_H
