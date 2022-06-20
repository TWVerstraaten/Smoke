//
// Created by pc on 20-6-22.
//

#include "Vector2F.h"

#include <cmath>

namespace app::math {

    Vector2F::Vector2F(float m_x, float m_y) : x(m_x), y(m_y) {
    }

    float Vector2F::length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2F& Vector2F::operator*(float multiplier) {
        x *= multiplier;
        y *= multiplier;
        return *this;
    }

    Vector2F& Vector2F::operator+=(const Vector2F& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2F& Vector2F::operator*=(float multiplier) {
        x *= multiplier;
        y *= multiplier;
        return *this;
    }

    Vector2F operator-(const Vector2F& a, const Vector2F& b) {
        return {a.x - b.x, a.y - b.y};
    }

    Vector2F operator+(const Vector2F& a, const Vector2F& b) {
        return {a.x + b.x, a.y + b.y};
    }

    Vector2F operator*(float multiplier, const Vector2F& vector) {
        return {multiplier * vector.x, multiplier * vector.y};
    }
} // namespace app::math