#ifndef H_FLUID_FLUID_CUH
#define H_FLUID_FLUID_CUH

#include "../app/math/Vector2F.h"
#include "FluidSettings.h"
#include "Matrix.h"

namespace app::fluid {

    class Fluid {

      public:
        Fluid();
        ~Fluid();

        void                        add_density(const app::math::Point& point, float multiplier = 1.0f);
        void                        add_velocity(const app::math::Point& point, const app::math::Direction& direction);
        void                        add_density(double x, double y, double multiplier = 1.0f);
        void                        add_velocity(double x, double y, double dx, double dy);
        void                        clear_previous();
        void                        clear_current();
        void                        step();
        void                        set_circle();
        void                        set_polygon(size_t n, double intensity, math::Point direction_offset);
        void                        scale_density(float multiplier);
        [[nodiscard]] float         sample_density_at(float x, float y) const;
        [[nodiscard]] float         sample_u_at(float x, float y) const;
        [[nodiscard]] float         sample_v_at(float x, float y) const;
        [[nodiscard]] const Matrix& u() const;
        [[nodiscard]] const Matrix& v() const;
        [[nodiscard]] const Matrix& density() const;

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
        float* m_density_cuda          = nullptr;
        float* m_density_previous_cuda = nullptr;
        float* m_u_cuda                = nullptr;
        float* m_v_cuda                = nullptr;
        float* m_u_previous_cuda       = nullptr;
        float* m_v_previous_cuda       = nullptr;
    };
} // namespace app::fluid

#endif