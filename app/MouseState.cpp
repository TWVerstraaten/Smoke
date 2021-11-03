//
// Created by pc on 01-11-21.
//

#include "MouseState.h"

#include <iostream>

namespace app {
    bool MouseState::pressed() const {
        return m_pressed;
    }

    sf::Vector2f MouseState::direction() const {
        return m_current - m_old;
    }

    void MouseState::press() {
        m_pressed = true;
    }

    void MouseState::release() {
        m_pressed = false;
    }

    void MouseState::get_new_position(const sf::RenderWindow& window) {
        const auto pos  = sf::Mouse::getPosition(window);
        const auto temp = sf::Vector2f{static_cast<float>(pos.x) / window.getSize().x, static_cast<float>(pos.y) / window.getSize().y};
        if (temp == m_current) {
            return;
        }
        move_current_to_old();
        m_current = temp;
        //        std::cout << m_old.x << ", " << m_old.y << " -> " << m_current.x << ", " << m_current.y << "\n";
    }

    void MouseState::move_current_to_old() {
        m_old = m_current;
    }

    const sf::Vector2f& MouseState::current() const {
        return m_current;
    }
} // namespace app
