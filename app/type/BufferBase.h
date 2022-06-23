//
// Created by pc on 07-11-21.
//

#ifndef H_APP_TYPE_BUFFERBASE_H
#define H_APP_TYPE_BUFFERBASE_H

#include "../math/Math.h"

#include <array>
#include <cassert>
#include <complex>
#include <cstddef>
#include <numeric>

namespace app::type {

    template <typename T, size_t Size>
    class BufferBase {

      public:
        static constexpr size_t s_size = Size;

        virtual const T& operator[](size_t index) const = 0;

        void clear() {
            m_data = {};
        }

        [[nodiscard]] T sum() const {
            assert(Size != 0);
            return std::accumulate(this->m_data.begin(), this->m_data.end(), T{});
        }

        [[nodiscard]] T average() const {
            assert(Size != 0);
            return sum() / Size;
        }

        [[nodiscard]] T energy(size_t start = 0, size_t end = s_size) const {
            T result{};
            for (size_t i = start; i != end; ++i)
                    result += math::square(std::abs(m_data[i]));

            return std::sqrt(result);
        }

        [[nodiscard]] const std::array<T, s_size>& data() const {
            return m_data;
        }

      protected:
        std::array<T, Size> m_data;
    };

} // namespace app::type

#endif // H_APP_TYPE_BUFFERBASE_H
