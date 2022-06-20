//
// Created by pc on 20-6-22.
//

#ifndef H_APP_DISP_AUTOMOVER_H
#define H_APP_DISP_AUTOMOVER_H

#include "../math/Vector2F.h"

namespace app::disp {

    class AutoMover {

      public:
        AutoMover();

        void update(float elapsed);
        void reset();
        [[nodiscard]] const app::math::Point& position() const;
        [[nodiscard]] const math::Direction&  velocity() const;
        [[nodiscard]] const math::Point&      target_position() const;

      private:
        static math::Point point_close_to_middle();

        app::math::Direction m_velocity;
        app::math::Point     m_position;
        app::math::Point     m_target_position;
    };

} // namespace app::disp

#endif // H_APP_DISP_AUTOMOVER_H
