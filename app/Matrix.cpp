//
// Created by pc on 03-11-21.
//

#include "Matrix.h"

namespace app {

    Matrix::Row<float> Matrix::operator[](size_t row) {
        return Row<float>{m_array.data() + g_point_count * row};
    }

    Matrix::Row<const float> Matrix::operator[](size_t row) const {
        return Row<const float>{m_array.data() + g_point_count * row};
    }

    void Matrix::set_zero() {
        m_array = {0};
    }
} // namespace app