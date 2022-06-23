//
// Created by pc on 05-11-21.
//

#ifndef H_APP_TYPE_LINEARBUFFER_H
#define H_APP_TYPE_LINEARBUFFER_H

#include "BufferBase.h"

#include <array>
#include <cassert>
#include <functional>

namespace app::type {

    template <typename T, size_t Size>
    class LinearBuffer : public BufferBase<T, Size> {

      public:
        LinearBuffer() = default;

        template <class S, size_t NewSize = Size>
        LinearBuffer<S, NewSize> cast(std::function<S(const T)> transform = [](const T t) { return S{t}; }) const {
            return LinearBuffer<S, NewSize>{static_cast<const T*>(this->m_data.data()), transform};
        }

        template <class S>
        explicit LinearBuffer(
            const S* source, std::function<T(const S)> transform = [](const S s) { return s; }, size_t length = Size) {
            assert(length <= Size);
            for (size_t i = 0; i != length; ++i)
                this->m_data[i] = transform(*(source + i));
            for (size_t i = length; i != Size; ++i)
                this->m_data[i] = T{};
        }

        [[nodiscard]] std::array<T, Size>& buffer() {
            return this->m_data;
        }

        [[nodiscard]] LinearBuffer<T, Size> operator+(const LinearBuffer<T, Size>& other) const {
            LinearBuffer<T, Size> result(*this);
            auto                  it       = result.m_data.begin();
            auto                  other_it = other.m_data.begin();

            for (; it != result.m_data.end(); ++it, ++other_it)
                *it += *other_it;

            return result;
        }

        [[nodiscard]] LinearBuffer<T, Size> operator/(float divider) const {
            LinearBuffer<T, Size> result(*this);
            for (auto& el : result.m_data)
                el /= divider;

            return result;
        }

        [[nodiscard]] const T& operator[](size_t index) const final {
            assert(index < Size);
            return this->m_data[index];
        }

        [[nodiscard]] T& operator[](size_t index) {
            assert(index < Size);
            return this->m_data[index];
        }
    };

} // namespace app::type

#endif // H_APP_TYPE_LINEARBUFFER_H
