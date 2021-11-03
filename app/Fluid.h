#ifndef H__FLUID_H
#define H__FLUID_H

#include "Matrix.h"
#include "Settings.h"

#include <array>
#include <iostream>
#include <vector>

namespace app {

    class Fluid {

      public:
        void                add_density(float x, float y, float multiplier = 1.0f);
        void                add_velocity(float x_direction, float y_direction, float x, float y);
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