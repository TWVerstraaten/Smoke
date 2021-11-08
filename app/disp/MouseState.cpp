//
// Created by pc on 08-11-21.
//

#include "MouseState.h"

namespace app::disp {

    void MouseState::press(Qt::MouseButton button) {
        switch (button) {
            case Qt::LeftButton:
                m_left_pressed = true;
                break;
            case Qt::RightButton:
                m_right_pressed = true;
                break;
            default:
                break;
        }
    }

    void MouseState::release(Qt::MouseButton button) {
        switch (button) {
            case Qt::LeftButton:
                m_left_pressed = false;
                break;
            case Qt::RightButton:
                m_right_pressed = false;
                break;
            default:
                break;
        }
    }

    bool MouseState::left_or_right_pressed() const {
        return m_left_pressed | m_right_pressed;
    }

    QVector2D MouseState::set_mouse_position(QVector2D mouse_new_position) {
        const auto direction = mouse_new_position - m_mouse_position;
        m_mouse_position     = mouse_new_position;
        return direction;
    }

    const QVector2D& MouseState::mouse_position() const {
        return m_mouse_position;
    }

    bool MouseState::left_pressed() const {
        return m_left_pressed;
    }

    bool MouseState::right_pressed() const {
        return m_right_pressed;
    }

} // namespace app::disp
