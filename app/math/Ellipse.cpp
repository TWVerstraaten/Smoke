//
// Created by pc on 18-11-21.
//

#include "Ellipse.h"

namespace app::math {

    Ellipse::EllipseBuilder::EllipseBuilder() : m_ellipse(new Ellipse) {
    }

    Ellipse Ellipse::EllipseBuilder::operator()() {
        return *m_ellipse;
    }

    Ellipse::EllipseBuilder& Ellipse::EllipseBuilder::set_axes(QPointF& axes) {
        m_ellipse->m_axes = axes;
        return *this;
    }
    Ellipse::EllipseBuilder& Ellipse::EllipseBuilder::set_center(QPointF& center) {
        m_ellipse->m_center = center;
        return *this;
    }

    Ellipse::EllipseBuilder& Ellipse::EllipseBuilder::set_rotation(float rotation) {
        m_ellipse->m_rotation = rotation;
        return *this;
    }

    Ellipse::EllipseBuilder& Ellipse::EllipseBuilder::set_theta_range(float theta_start, float theta_end) {
        m_ellipse->m_theta_start = theta_start;
        m_ellipse->m_theta_end   = theta_end;
        return *this;
    }

    Ellipse::EllipseBuilder Ellipse::build() {
        return {};
    }
} // namespace app::math