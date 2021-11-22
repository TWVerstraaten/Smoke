//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_SMOKERENDERER_H
#define H_APP_DISP_SMOKERENDERER_H

#include "RendererBase.h"

namespace app::fl {
    class Fluid;
}

namespace app::disp {

    class SmokeRenderer : public RendererBase {

      public:
        void fill(const app::fl::Fluid& fluid);
        void set_sample_points(size_t width, size_t height);

      private:
        void fill_quads_normal(const app::fl::Fluid& fluid);
        void fill_quads_pixel(const app::fl::Fluid& fluid);

        size_t m_vertical_sample_points{};
        size_t m_horizontal_sample_points{};
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKERENDERER_H
