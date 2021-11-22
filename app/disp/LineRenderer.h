//
// Created by pc on 18-11-21.
//

#ifndef H_APP_DISP_LINERENDERER_H
#define H_APP_DISP_LINERENDERER_H

#include "RendererBase.h"

namespace app::disp {

    class LineRenderer : public RendererBase {

      public:
        LineRenderer();

        void fill_around_circle(const std::vector<size_t>& buffer);
        void fill_around_ellipse(const std::vector<size_t>& buffer, float a, float b, float theta, float dx, float dy);
    };

} // namespace app::disp

#endif // H_APP_DISP_LINERENDERER_H
