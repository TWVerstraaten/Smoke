//
// Created by pc on 16-11-21.
//

#include "ScopedTimer.h"

#include <iostream>
#include <utility>

namespace app::tools {

    ScopedTimer::ScopedTimer(std::string name, std::string delimiter)
        : m_name(std::move(name)), m_start(std::chrono::high_resolution_clock::now()), m_delimiter(std::move(delimiter)) {
    }

    ScopedTimer::~ScopedTimer() {
        const auto stop = (std::chrono::high_resolution_clock::now());
        std::cout << m_name << ":\t" << std::chrono::duration_cast<std::chrono::microseconds>(stop - m_start).count() << "ms" << m_delimiter;
    }

} // namespace app::tools
