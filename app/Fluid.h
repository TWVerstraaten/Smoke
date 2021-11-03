#ifndef H_APP_FLUID_H
#define H_APP_FLUID_H

#include "Matrix.h"
#include "Settings.h"

#include <SFML/System/Vector2.hpp>

namespace app {

    class Fluid {

      public:
        void                add_density(sf::Vector2f position, float multiplier = 1.0f);
        void                add_velocity(sf::Vector2f position, sf::Vector2f direction);
        void                density_step(float dt);
        void                velocity_step(float dt);
        void                clear_previous();
        void                clear_current();
        void                step(float dt);
        [[nodiscard]] float sample_at(float x, float y) const;

      private:
        Matrix m_u{};
        Matrix m_u_previous{};
        Matrix m_v{};
        Matrix m_v_previous{};
        Matrix m_density{};
        Matrix m_density_previous{};
    };

} // namespace app

#endif