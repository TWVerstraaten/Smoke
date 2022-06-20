//
// Created by pc on 07-11-21.
//

#ifndef H_AUDIO_BUFFERBASE_H
#define H_AUDIO_BUFFERBASE_H

#include <array>
#include <cassert>
#include <complex>
#include <cstddef>
#include <numeric>

namespace app::audio {

    template <typename T, size_t Size>
    class BufferBase {

      public:
        static constexpr size_t size = Size;

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

        [[nodiscard]] T energy() const {
            T result{};
            for (size_t i = 0; i != size; ++i)
                result += std::abs(m_data[i] * m_data[i]);

            return result;
        }

        [[nodiscard]] const std::array<T, size>& data() const {
            return m_data;
        }


      protected:
        std::array<T, Size> m_data;
    };

} // namespace audio

#endif // H_AUDIO_BUFFERBASE_H
