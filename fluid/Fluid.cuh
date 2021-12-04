#ifndef H_FLUID_FLUID_CUH
#define H_FLUID_FLUID_CUH

#include "FluidSettings.h"
#include "Matrix.h"

namespace app {
    namespace fluid {

        class Fluid {

          public:
            Fluid();
            ~Fluid();

            void                add_bar();
            void                add_density(float x, float y, float multiplier = 1.0f);
            void                add_velocity(float x, float y, float dx, float dy);
            void                clear_previous();
            void                clear_current();
            void                step();
            void                set_circle();
            [[nodiscard]] float sample_density_at(float x, float y) const;
            [[nodiscard]] float sample_u_at(float x, float y) const;
            [[nodiscard]] float sample_v_at(float x, float y) const;

          private:
            void density_step();
            void velocity_step();

            void send_to_gpu();
            void get_from_gpu();

            Matrix m_u{};
            Matrix m_u_previous{};
            Matrix m_v{};
            Matrix m_v_previous{};
            Matrix m_density{};
            Matrix m_density_previous{};

            // Pointers for CUDA
            float* m_density_gpu;
            float* m_density_previous_gpu;
            float* m_u_gpu;
            float* m_v_gpu;
            float* m_u_previous_gpu;
            float* m_v_previous_gpu;
        };
    } // namespace fluid
} // namespace app

#endif