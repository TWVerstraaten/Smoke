//
// Created by pc on 03-11-21.
//

#ifndef H_APP_MATH_MATH_H
#define H_APP_MATH_MATH_H

#include "IntFrac.h"
#include "Vector2F.h"

#include <cstddef>

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
    bool     bernoulli_trial(float p);
    float    uniform_zero_one();
    int      random_number_in_range(int min, int max);

} // namespace app::math

#endif // H_APP_MATH_MATH_H
