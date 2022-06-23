//
// Created by pc on 20-6-22.
//

#ifndef H_APP_MATH_VECTOR2F_H
#define H_APP_MATH_VECTOR2F_H

namespace app::math {

    struct Vector2F {
        Vector2F() = default;
        Vector2F(double m_x, double m_y);

        double x = 0.0f;
        double y = 0.0f;

        [[nodiscard]] double length() const;

        Vector2F& operator+=(const Vector2F& other);
        Vector2F& operator*(double multiplier);
        Vector2F& operator*=(double multiplier);
    };

    typedef Vector2F Point;
    typedef Vector2F Direction;

    Vector2F operator-(const Vector2F& a, const Vector2F& b);
    Vector2F operator+(const Vector2F& a, const Vector2F& b);
    Vector2F operator*(double multiplier, const Vector2F& vector);

} // namespace app::math

#endif // H_APP_MATH_VECTOR2F_H
