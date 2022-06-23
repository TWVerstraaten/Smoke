//
// Created by pc on 03-11-21.
//

#include "Math.h"

#include <cmath>
#include <random>

namespace app::math {

    void rotateAboutZero(Vector2F& point, float theta) {
        const float c = std::cos(theta);
        const float s = std::sin(theta);
        point         = {c * point.x - s * point.y, s * point.x + c * point.y};
    }

    Vector2F random_point_zero_one() {
        return {uniform_zero_one(), uniform_zero_one()};
    }

    Vector2F random_point_centered() {
        return 2 * random_point_zero_one() - Point{1, 1};
    }

    bool bernoulli_trial(float p) {
        return uniform_zero_one() < p;
    }

    float uniform_zero_one() {
        static std::default_random_engine            generator(2);
        static std::uniform_real_distribution<float> distribution(0.0, 1.0);

        return distribution(generator);
    }

    int random_number_in_range(int min, int max) {
        static std::random_device          random_device;
        static std::default_random_engine  engine(random_device());
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(engine);
    }

} // namespace app::math
