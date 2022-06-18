//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_MOUSESTATE_H
#define H_APP_DISP_MOUSESTATE_H

#include <QPointF>

namespace app::disp {

    class MouseState {

      public:
        void                         press(Qt::MouseButton button);
        void                         release(Qt::MouseButton button);
        void                         set_mouse_position(const QPointF& mouse_new_position);
        [[nodiscard]] const QPointF& mouse_position() const;
        [[nodiscard]] bool           any_pressed() const;
        [[nodiscard]] bool           left_pressed() const;
        [[nodiscard]] bool           right_pressed() const;

      private:
        QPointF m_mouse_position;
        bool    m_left_pressed  = false;
        bool    m_right_pressed = false;
    };

} // namespace app::disp

#endif // H_APP_DISP_MOUSESTATE_H
