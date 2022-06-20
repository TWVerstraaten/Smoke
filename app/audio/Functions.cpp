//
// Created by pc on 07-11-21.
//

#include "Functions.h"

namespace app::audio {

    std::function<Complex(const float)> float_to_complex() {
        return [](const float t) { return Complex(t, 0.0); };
    }

    std::function<float(const Complex)> complex_to_dB() {
        return [](const Complex t) { return 40 * (std::max(0.0, std::log((6 * std::abs(t.real()))))); };
    }

} // namespace audio
