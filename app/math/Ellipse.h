//
// Created by pc on 18-11-21.
//

#ifndef H_APP_MATH_ELLIPSE_H
#define H_APP_MATH_ELLIPSE_H

#include <QPointF>
#include <cmath>
#include <memory>

namespace app::math {

    class Ellipse {

      public:
        class EllipseBuilder {
            friend class Ellipse;

          public:
            Ellipse         operator()();
            EllipseBuilder& set_axes(QPointF& axes);
            EllipseBuilder& set_center(QPointF& center);
            EllipseBuilder& set_rotation(float rotation);
            EllipseBuilder& set_theta_range(float theta_start, float theta_end);

          private:
            EllipseBuilder();

            std::unique_ptr<Ellipse> m_ellipse;
        };

        static EllipseBuilder build();

      private:
        Ellipse() = default;

        QPointF m_center      = {0.0f, 0.0f};
        QPointF m_axes        = {1.0f, 1.0f};
        float   m_rotation    = 0.0f;
        float   m_theta_start = 0.0f;
        float   m_theta_end   = 2.0f * M_PI;
    };

} // namespace app::math

#endif // H_APP_MATH_ELLIPSE_H
