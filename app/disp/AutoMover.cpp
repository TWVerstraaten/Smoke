//
// Created by pc on 20-6-22.
//

#include "AutoMover.h"

#include "../math/Math.h"

namespace app::disp {

    AutoMover::AutoMover()
        : m_velocity(0.5 * (math::random_point_zero_one() - math::Vector2F{0.5, 0.5})), m_position(app::math::random_point_zero_one()), m_target_position(point_close_to_middle()) {
    }

    void AutoMover::update([[maybe_unused]] float elapsed) {
        auto target_direction = m_target_position - m_position;

        m_velocity = 0.99 * m_velocity + 3 * elapsed * target_direction + 0.05 * (math::random_point_zero_one() - math::Vector2F{0.5, 0.5});
        if (m_velocity.length() >= 3.5f)
            m_velocity *= 3.5f / m_velocity.length();

        m_position += 2.0 * elapsed * m_velocity;
        if ((m_position - m_target_position).length() < 0.1)
            reset();

        m_target_position += -0.01 * m_velocity;
    }

    void AutoMover::reset() {
        m_target_position = point_close_to_middle();
        m_velocity += 0.3 * (math::Vector2F{0.5, 0.5} - m_position);
    }

    const app::math::Vector2F& AutoMover::position() const {
        return m_position;
    }

    const math::Point& AutoMover::target_position() const {
        return m_target_position;
    }

    const math::Direction& AutoMover::velocity() const {
        return m_velocity;
    }
    math::Point AutoMover::point_close_to_middle() {
        return 0.75 * app::math::random_point_zero_one() + math::Point{0.125, 0.125};
    }

} // namespace app::disp