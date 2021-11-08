//
// Created by pc on 03-11-21.
//

#include "Matrix.h"

#include "../Math.h"

#include <cassert>

namespace app::fl {

    Matrix::Row<float> Matrix::operator[](size_t row) {
        return Row<float>{m_array.data() + g_point_count * row};
    }

    Matrix::Row<const float> Matrix::operator[](size_t row) const {
        return Row<const float>{m_array.data() + g_point_count * row};
    }

    void Matrix::set_zero() {
        m_array = {0};
    }

    float Matrix::sample_at(float x, float y) const {
        assert(x <= 1.0f);
        assert(x >= 0.0f);
        assert(y <= 1.0f);
        assert(y >= 0.0f);
        const auto [i, f_i] = app::math::mod_one(x * (g_point_count - 1));
        const auto [j, f_j] = app::math::mod_one(y * (g_point_count - 1));
        // Bilinear interpolation
        return (*this)[i][j] * (1.0f - f_i) * (1.0f - f_j) + (*this)[i + 1][j] * f_i * (1.0f - f_j) + (*this)[i + 1][j + 1] * f_i * f_j + (*this)[i][j + 1] * (1.0f - f_i) * f_j;
    }
} // namespace app::fl