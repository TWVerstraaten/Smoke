//
// Created by pc on 11-11-21.
//

#ifndef H_APP_DISP_SHADERSETTINGS_H
#define H_APP_DISP_SHADERSETTINGS_H

#include <cstddef>

namespace app::disp {

    enum COLOR_MODE { GRAY = 0, RGB = 1, SPACE = 2 };

    extern float      g_zoom_depth;
    extern float      g_zoom_decay;
    extern bool       g_invert_colors;
    extern bool       g_clamp_colors;
    extern size_t     g_clamp_count;
    extern COLOR_MODE g_color_mode;

} // namespace app::disp

#endif // H_APP_DISP_SHADERSETTINGS_H
