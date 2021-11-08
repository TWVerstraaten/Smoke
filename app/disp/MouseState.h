//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_MOUSESTATE_H
#define H_APP_DISP_MOUSESTATE_H

#include <QVector2D>
#include <qnamespace.h>

namespace app::disp {

    class MouseState {

      public:
        void                           press(Qt::MouseButton button);
        void                           release(Qt::MouseButton button);
        QVector2D                      set_mouse_position(QVector2D mouse_new_position);
        [[nodiscard]] const QVector2D& mouse_position() const;
        [[nodiscard]] bool             left_or_right_pressed() const;
        [[nodiscard]] bool             left_pressed() const;
        [[nodiscard]] bool             right_pressed() const;

      private:
        QVector2D m_mouse_position;
        bool      m_left_pressed  = false;
        bool      m_right_pressed = false;
    };

} // namespace app::disp

#endif // H_APP_DISP_MOUSESTATE_H
