//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_SMOKERENDERER_H
#define H_APP_DISP_SMOKERENDERER_H

#include "RendererBase.h"

namespace app::fluid {
    class Fluid;
}

namespace app::disp {

    class SmokeRenderer : public RendererBase {

      public:
        SmokeRenderer();

        void fill(const app::fluid::Fluid& fluid);
        void set_sample_points(size_t width, size_t height);
        void resize(size_t width, size_t height);

      private:
        void fill_quads_pixel(const app::fluid::Fluid& fluid);
        void fill_quad_pixel_thread(size_t start, size_t end, const fluid::Fluid& fluid);

        size_t m_vertical_sample_points   = 0;
        size_t m_horizontal_sample_points = 0;
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKERENDERER_H
