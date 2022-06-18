//
// Created by pc on 16-11-21.
//

#include "ScopedTimer.h"

#include <iostream>
#include <utility>

namespace app::prf {

    ScopedTimer::ScopedTimer(std::string name, std::string delimiter, bool print_on_destruct)
        : m_name(std::move(name)), m_start(std::chrono::high_resolution_clock::now()), m_delimiter(std::move(delimiter)), m_print_on_destruct(print_on_destruct) {
    }

    ScopedTimer::~ScopedTimer() {
        if (m_print_on_destruct) {
            const auto stop = (std::chrono::high_resolution_clock::now());
            std::cout << m_name << ":\t" << std::chrono::duration_cast<std::chrono::microseconds>(stop - m_start).count() << "ms" << m_delimiter;
        }
    }

    size_t ScopedTimer::elapsed() const {
        const auto stop = (std::chrono::high_resolution_clock::now());
        return std::chrono::duration_cast<std::chrono::microseconds>(stop - m_start).count();
    }

    const std::string& ScopedTimer::name() const {
        return m_name;
    }

} // namespace app::prf
