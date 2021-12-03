//
// Created by pc on 03-11-21.
//

#include "Math.h"

#include <cmath>

namespace app::tools::math {

    std::pair<int, float> mod_one(const float x) {
        float i;
        float f = std::modf(x, &i);
        return {std::round(i), f};
    }

    Point2F rotate(const Point2F& point, float theta) {
        float c = std::cos(theta);
        float s = std::sin(theta);
        return {c * point.x - s * point.y, s * point.x + c * point.y};
    }

} // namespace app::tools::math
