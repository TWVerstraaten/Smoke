//
// Created by pc on 05-12-21.
//

#ifndef H_APP_TYPE_BUFFERTYPES_H
#define H_APP_TYPE_BUFFERTYPES_H

#include <cstddef>

namespace app::type {
    template <typename T, size_t Size>
    class CircularBuffer;
    template <typename T, size_t Size>
    class LinearBuffer;

    typedef LinearBuffer<double, 256>     AudioBuffer;
    typedef LinearBuffer<double, 64>      DftBuffer;
    typedef CircularBuffer<double, 500>   HistoryBuffer;
    typedef CircularBuffer<DftBuffer, 8>  DftBufferBuffer;
    typedef CircularBuffer<DftBuffer, 32> DftAverageBuffer;
} // namespace app::type

#endif // H_APP_TYPE_BUFFERTYPES_H
