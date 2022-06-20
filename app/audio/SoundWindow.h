//
// Created by pc on 05-11-21.
//

#ifndef H_APP_AUDIO_SOUNDWINDOW_H
#define H_APP_AUDIO_SOUNDWINDOW_H

#include "BeatHandler.h"
#include "CircularBuffer.h"
#include "Complex.h"
#include "LinearBuffer.h"
#include "Types.h"

#include <SFML/System/Clock.hpp>

namespace app::audio {

    class AudioPlayer;

    class SoundWindow {

      public:
        SoundWindow();

        void                                  update(const AudioPlayer& player);
        void                                  clear();
        [[nodiscard]] float                   current_beat_intensity() const;
        [[nodiscard]] const HistoryBuffer&    waveform() const;
        [[nodiscard]] const HistoryBuffer&    energy_buffer() const;
        [[nodiscard]] const HistoryBuffer&    energy_dif_buffer() const;
        [[nodiscard]] const DftAverageBuffer& smoothed_dft_buffers() const;

      private:
        void handle_energy();

        HistoryBuffer    m_waveform{};
        HistoryBuffer    m_energy_buffer{};
        HistoryBuffer    m_energy_dif_buffer{};
        DftBufferBuffer  m_dft_buffers{};
        DftAverageBuffer m_averaged_dft_buffers{};
        BeatHandler      m_beat_handler;
    };

} // namespace app::audio

#endif // H_APP_AUDIO_SOUNDWINDOW_H
