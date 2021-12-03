//
// Created by pc on 03-11-21.
//

#ifndef H_APP_MATH_H
#define H_APP_MATH_H

#include "Point2F.h"

#include <utility>

namespace app::tools::math {

    template <typename T>
    T clamp(T value, T min, T max) {
        return value <= min ? min : value >= max ? max : value;
    }

    template <typename T>
    static T square(T a) {
        return a * a;
    }

    std::pair<int, float> mod_one(float x);
    Point2F               rotate(const Point2F& point, float theta);

} // namespace app::tools::math

#endif // H_APP_MATH_H
