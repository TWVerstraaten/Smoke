//
// Created by pc on 03-11-21.
//

#include "Math.h"

#include <cmath>

namespace app::math {

    void rotateAboutZero(QPointF& point, float theta) {
        const float c = std::cos(theta);
        const float s = std::sin(theta);
        point         = {c * point.x() - s * point.y(), s * point.x() + c * point.y()};
    }

} // namespace app::math
