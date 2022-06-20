//
// Created by pc on 06-11-21.
//

#ifndef H_TOOLS_FUNCTIONS_H
#define H_TOOLS_FUNCTIONS_H

#include "Complex.h"

#include <functional>

namespace app::audio {

    std::function<Complex(const float)> float_to_complex();

    std::function<float(const Complex)> complex_to_dB();

} // namespace audio

#endif // H_TOOLS_FUNCTIONS_H
