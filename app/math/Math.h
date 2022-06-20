//
// Created by pc on 03-11-21.
//

#ifndef H_APP_MATH_MATH_H
#define H_APP_MATH_MATH_H

#include "IntFrac.h"
#include "Vector2F.h"

namespace app::math {

    template <typename T>
    T clamp(T value, T min, T max) {
        return value <= min ? min : value >= max ? max : value;
    }

    template <typename T>
    static T square(T a) {
        return a * a;
    }

    void     rotateAboutZero(Vector2F& point, float theta);
    Vector2F random_point_zero_one();
    Vector2F random_point_centered();

} // namespace app::math

#endif // H_APP_MATH_MATH_H
