//
// Created by pc on 01-11-21.
//

#ifndef H_APP_MOUSESTATE_H
#define H_APP_MOUSESTATE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
namespace app {

    class MouseState {

      public:
        [[nodiscard]] const sf::Vector2f &current() const;
        [[nodiscard]] bool pressed() const;
        [[nodiscard]] sf::Vector2f direction() const;

        void press();
        void release();
        void get_new_position(const sf::RenderWindow &window);

      private:
        void move_current_to_old();


        sf::Vector2f m_old;
        sf::Vector2f m_current;
        bool m_pressed = false;
    };

}// namespace app


#endif//H_APP_MOUSESTATE_H
