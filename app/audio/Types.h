//
// Created by pc on 05-12-21.
//

#ifndef H_APP_AUDIO_TYPES_H
#define H_APP_AUDIO_TYPES_H

#include <cstddef>

namespace app::audio {
    template <typename T, size_t Size>
    class CircularBuffer;
    template <typename T, size_t Size>
    class LinearBuffer;

    typedef LinearBuffer<float, 512>      AudioBuffer;
    typedef LinearBuffer<float, 128>      DftBuffer;
    typedef CircularBuffer<float, 1500>   HistoryBuffer;
    typedef CircularBuffer<DftBuffer, 28> DftBufferBuffer;
    typedef CircularBuffer<DftBuffer, 5>  DftAverageBuffer;
} // namespace app::audio

#endif // H_APP_AUDIO_TYPES_H
