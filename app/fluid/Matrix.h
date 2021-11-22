//
// Created by pc on 03-11-21.
//

#ifndef H_APP_FLUID_MATRIX_H
#define H_APP_FLUID_MATRIX_H

#include "FlSettings.h"

#include <array>
#include <cassert>

namespace app::fluid {

    class Matrix {

      public:
        template <class T>
        struct Row {
            T& operator[](size_t index) const {
                assert(index < g_point_count);
                return *(m_p + index);
            }
            T* m_p;
        };
        [[nodiscard]] Row<float>       operator[](size_t row);
        [[nodiscard]] Row<const float> operator[](size_t row) const;

        void                set_zero();
        void                add_scaled(const Matrix& rhs, float weight);
        Matrix&             operator+=(const Matrix& rhs);
        [[nodiscard]] float sample_at(float x, float y) const;

      private:
        std::array<float, g_point_count * g_point_count> m_array{};
    };

} // namespace app::fluid

#endif // H_APP_FLUID_MATRIX_H
