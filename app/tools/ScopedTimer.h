//
// Created by pc on 16-11-21.
//

#ifndef H_APP_TOOLS_SCOPEDTIMER_H
#define H_APP_TOOLS_SCOPEDTIMER_H

#include <chrono>
#include <string>

namespace app::tools {

    class ScopedTimer {
        typedef decltype(std::chrono::high_resolution_clock::now()) TimePoint;

      public:
        explicit ScopedTimer(std::string name, std::string delimiter = "\t\t", bool print_on_destruct = false);
        ~ScopedTimer();

        [[nodiscard]] size_t             elapsed() const;
        [[nodiscard]] const std::string& name() const;

      private:
        std::string m_name;
        TimePoint   m_start;
        std::string m_delimiter;
        bool        m_print_on_destruct;
    };

} // namespace app::tools

#endif // H_APP_TOOLS_SCOPEDTIMER_H
