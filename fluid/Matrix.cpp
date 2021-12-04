//
// Created by pc on 03-11-21.
//

#include "Matrix.h"

#include "../app/tools/Math.h"

#include <cassert>

namespace app {
    namespace fluid {

        Matrix::Row<float> Matrix::operator[](size_t row) {
            assert(row < g_point_count);
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
            const auto ri  = app::tools::math::mod_one(x * (g_point_count - 1));
            const auto rj  = app::tools::math::mod_one(y * (g_point_count - 1));
            const auto i   = ri.first;
            const auto f_i = ri.second;
            const auto j   = rj.first;
            const auto f_j = rj.second;

            // Bilinear interpolation
            return (*this)[i][j] * (1.0f - f_i) * (1.0f - f_j) + (*this)[i + 1][j] * f_i * (1.0f - f_j) + (*this)[i + 1][j + 1] * f_i * f_j +
                   (*this)[i][j + 1] * (1.0f - f_i) * f_j;
        }

        Matrix& Matrix::operator+=(const Matrix& rhs) {
            assert(m_array.size() == rhs.m_array.size());
            const auto& rhs_array = rhs.m_array;
            auto        rhs_it    = rhs_array.cbegin();
            for (auto it = m_array.begin(); it != m_array.end(); ++it, ++rhs_it) {
                *it += *rhs_it;
            }
            return *this;
        }

        void Matrix::add_scaled(const Matrix& rhs, float weight) {
            assert(m_array.size() == rhs.m_array.size());
            const auto& rhs_array = rhs.m_array;
            auto        rhs_it    = rhs_array.cbegin();
            for (auto it = m_array.begin(); it != m_array.end(); ++it, ++rhs_it) {
                *it += weight * (*rhs_it);
            }
        }

        const float* Matrix::data() const {
            return m_array.data();
        }

        float* Matrix::data() {
            return m_array.data();
        }
    } // namespace fluid
} // namespace app