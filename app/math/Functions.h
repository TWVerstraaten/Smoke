//
// Created by pc on 06-11-21.
//

#ifndef H_APP_MATH_FUNCTIONS_H
#define H_APP_MATH_FUNCTIONS_H

#include "../type/Complex.h"

#include <functional>

namespace app::math {

    std::function<Complex(const double)> double_to_complex();

    std::function<double(const Complex)> complex_to_dB();

} // namespace app::math

#endif // H_APP_MATH_FUNCTIONS_H
