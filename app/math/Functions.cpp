//
// Created by pc on 07-11-21.
//

#include "Functions.h"

namespace app::math {

    std::function<Complex(const double)> double_to_complex() {
        return [](const double t) { return Complex(t, 0.0); };
    }

    //    std::function<double(const Complex)> complex_to_dB() {
    //        return [](const Complex t) { return std::abs(t); };
    //    }
    std::function<double(const Complex)> complex_to_dB() {
        return [](const Complex t) { return 100 * std::log10(1.0 + 2* std::abs(t.real())); };
    }

} // namespace app::math
