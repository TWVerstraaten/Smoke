//
// Created by pc on 21-6-22.
//

#ifndef H_APP_DISP_SMOKEEFFECT_H
#define H_APP_DISP_SMOKEEFFECT_H

#include <functional>
#include <map>

namespace app::disp {

    class SmokeWidget;

    typedef std::function<void(SmokeWidget& smoke_widget, double intensity)> SmokeEffect;

    struct SmokeEffectDetail {
        SmokeEffect smoke_effect;
        double      probability;
        double      cool_down_period = 0.0;
        bool        is_on_cool_down  = false;
        void        start_cool_down();
    };

    class SmokeEffectManager {

      public:
        static std::vector<SmokeEffectDetail> s_effects;

      private:
        static SmokeEffectDetail clamp_coefficient_slow_upwards();
        static SmokeEffectDetail set_polygon();
        static SmokeEffectDetail switch_color_mode();
        static SmokeEffectDetail invert_colors();
        static SmokeEffectDetail clamp_coefficient_slow_downwards();
        static SmokeEffectDetail clamp_count_fast();
        static SmokeEffectDetail power_scale_fast();
        static SmokeEffectDetail clamp_coefficient_fast();
        static SmokeEffectDetail pixel_size();
        static SmokeEffectDetail color_wash_up();
        static SmokeEffectDetail color_wash_down();
        static SmokeEffectDetail time_slow_down();
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKEEFFECT_H
