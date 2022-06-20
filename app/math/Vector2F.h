//
// Created by pc on 20-6-22.
//

#ifndef H_APP_MATH_VECTOR2F_H
#define H_APP_MATH_VECTOR2F_H

namespace app::math {

    struct Vector2F {
        Vector2F() = default;
        Vector2F(float m_x, float m_y);

        float x = 0.0f;
        float y = 0.0f;

        [[nodiscard]] float length() const;

        Vector2F& operator+=(const Vector2F& other);
        Vector2F& operator*(float multiplier);
        Vector2F& operator*=(float multiplier);
    };

    typedef Vector2F Point;
    typedef Vector2F Direction;

    Vector2F operator-(const Vector2F& a, const Vector2F& b);
    Vector2F operator+(const Vector2F& a, const Vector2F& b);
    Vector2F operator*(float multiplier, const Vector2F& vector);

} // namespace app::math

#endif // H_APP_MATH_VECTOR2F_H
