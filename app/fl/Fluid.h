#ifndef H_APP_FL_FLUID_H
#define H_APP_FL_FLUID_H

#include "Matrix.h"
#include "Settings.h"

#include <SFML/System/Vector2.hpp>

namespace app::fl {

    class Fluid {

      public:
        void                add_bar();
        void                add_density(float x, float y, float multiplier = 1.0f);
        void                add_velocity(float x, float y, float dx, float dy);
        void                clear_previous();
        void                clear_current();
        void                step(float dt);
        [[nodiscard]] float sample_density_at(float x, float y) const;
        [[nodiscard]] float sample_u_at(float x, float y) const;
        [[nodiscard]] float sample_v_at(float x, float y) const;

        void set_circle();

      private:
        void density_step(float dt);
        void velocity_step(float dt);
        void decrease_density();

        Matrix m_u{};
        Matrix m_u_previous{};
        Matrix m_v{};
        Matrix m_v_previous{};
        Matrix m_density{};
        Matrix m_density_previous{};
    };

} // namespace app::fl

#endif