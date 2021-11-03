//
// Created by pc on 03-11-21.
//

#ifndef H_APP_MATH_H
#define H_APP_MATH_H

namespace app::math {

    template <typename T>
    T clamp(T value, T min, T max) {
        return value < min ? min : value > max ? max : value;
    }

} // namespace app::math

#endif // H_APP_MATH_H
