//
// Created by pc on 18-11-21.
//

#ifndef H_APP_TOOLS_ELLIPSE_H
#define H_APP_TOOLS_ELLIPSE_H

#include "Point2F.h"

#include <cmath>
#include <memory>

namespace app::tools {

    class Ellipse {

      public:
        class EllipseBuilder {
            friend class Ellipse;

          public:
            Ellipse         operator()();
            EllipseBuilder& set_axes(Point2F& axes);
            EllipseBuilder& set_center(Point2F& center);
            EllipseBuilder& set_rotation(float rotation);
            EllipseBuilder& set_theta_range(float theta_start, float theta_end);

          private:
            EllipseBuilder();

            std::unique_ptr<Ellipse> m_ellipse;
        };

        static EllipseBuilder build() {
            return {};
        }

      private:
        //        static Ellipse doit() {
        //            return build().set_axes({2, 2}).set_center({3, 3});
        //        }

        Ellipse() = default;

        Point2F m_center      = {0.0f, 0.0f};
        Point2F m_axes        = {1.0f, 1.0f};
        float   m_rotation    = 0.0f;
        float   m_theta_start = 0.0f;
        float   m_theta_end   = 2.0f * M_PI;
    };

} // namespace app::tools

#endif // H_APP_TOOLS_ELLIPSE_H
