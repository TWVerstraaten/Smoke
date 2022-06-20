//
// Created by pc on 03-11-21.
//

#include "Math.h"

#include <cmath>

namespace app::math {

    void rotateAboutZero(Vector2F& point, float theta) {
        const float c = std::cos(theta);
        const float s = std::sin(theta);
        point         = {c * point.x - s * point.y, s * point.x + c * point.y};
    }

    Vector2F random_point_zero_one() {
        auto x = rand() / static_cast<float>(RAND_MAX);
        auto y = rand() / static_cast<float>(RAND_MAX);

        return {x, y};
    }

    Vector2F random_point_centered() {
        return 2 * random_point_zero_one() - Point{1, 1};
    }

} // namespace app::math
