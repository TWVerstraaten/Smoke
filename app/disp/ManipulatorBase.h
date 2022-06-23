//
// Created by pc on 20-6-22.
//

#ifndef H_APP_DISP_MANIPULATORBASE_H
#define H_APP_DISP_MANIPULATORBASE_H

#include "DispSettings.h"

#include <cassert>
#include <iostream>
#include <variant>

namespace app::disp {

    template <typename T>
    class ManipulatorBase {
      public:
        ManipulatorBase(float duration_left, T start, T end, T* value) : m_duration_left(duration_left), m_value(value), m_end(end), m_current(start) {
            *value     = start;
            m_gradient = (static_cast<float>(end) - static_cast<float>(start)) / m_duration_left;
        }

        ManipulatorBase<T>& operator=(const ManipulatorBase<T>& other) = default;

        ~ManipulatorBase() = default;

        void update(float elapsed) {
            assert(not m_is_done);
            m_duration_left -= elapsed;
            m_current += elapsed * m_gradient;
            *m_value = m_current;
            if (m_duration_left <= 0) {
                m_is_done = true;
                *m_value  = m_end;
            }
        }

        [[nodiscard]] bool is_done() const {
            return m_is_done;
        }

        template <class S>
        bool manipulating_same(const ManipulatorBase<S>& other) const {
            return static_cast<void*>(m_value) == static_cast<void*>(other.value());
        }

        T* value() const {
            return m_value;
        }

      private:
        bool  m_is_done = false;
        float m_duration_left;

        T*    m_value;
        float m_end;
        float m_gradient;
        float m_current;
    };

    typedef std::variant<ManipulatorBase<float>, ManipulatorBase<size_t>> Manipulator;

} // namespace app::disp

#endif // H_APP_DISP_MANIPULATORBASE_H
