//
// Created by pc on 03-11-21.
//

#include "Math.h"

#include <cmath>

namespace app::math {

    std::pair<int, float> mod_one(const float x) {
        float i;
        float f = std::modf(x, &i);
        return {std::round(i), f};
    }

    std::pair<float, float> rotate(float x, float y, float theta) {
        float c = std::cos(theta);
        float s = std::sin(theta);
        return {c * x - s * y, s * x + c * y};
    }

} // namespace app::math
