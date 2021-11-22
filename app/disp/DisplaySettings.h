//
// Created by pc on 17-11-21.
//

#ifndef H_APP_DISP_DISPLAYSETTINGS_H
#define H_APP_DISP_DISPLAYSETTINGS_H

#include <cstddef>

namespace app::disp {
    enum class PIXEL_MODE { NORMAL, PIXEL };

    extern PIXEL_MODE g_pixel_mode;
    extern size_t     g_vertical_sample_points;
    extern size_t     g_horizontal_sample_points;

} // namespace app::disp

#endif // H_APP_DISP_DISPLAYSETTINGS_H
