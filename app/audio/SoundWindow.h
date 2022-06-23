//
// Created by pc on 05-11-21.
//

#ifndef H_APP_AUDIO_SOUNDWINDOW_H
#define H_APP_AUDIO_SOUNDWINDOW_H

#include "../type/BufferTypes.h"
#include "../type/CircularBuffer.h"
#include "../type/Complex.h"
#include "../type/LinearBuffer.h"
#include "BeatHandler.h"

#include <SFML/System/Clock.hpp>

namespace app::audio {

    class AudioPlayer;

    class SoundWindow {

      public:
        SoundWindow();

        void                                        update_by_player(const AudioPlayer& player);
        [[nodiscard]] float                         current_beat_intensity() const;
        [[nodiscard]] const type::HistoryBuffer&    waveform() const;
        [[nodiscard]] const type::DftAverageBuffer& smoothed_dft_buffers() const;
        [[nodiscard]] const type::DftBufferBuffer&  dft_buffers() const;
        [[nodiscard]] const type::HistoryBuffer&    low_beat_buffer() const;
        [[nodiscard]] const type::HistoryBuffer&    mid_beat_buffer() const;
        [[nodiscard]] const type::HistoryBuffer&    high_beat_buffer() const;

      private:
        void process_buffer(const type::AudioBuffer& buffer);
        void handle_energy(const type::AudioBuffer& buffer);
        void set_waveform_value(const type::AudioBuffer& buffer);
        double energy_difference(size_t i_start, size_t i_end);

        type::HistoryBuffer    m_waveform{};
        type::HistoryBuffer    m_energy_buffer{};
        type::HistoryBuffer    m_energy_dif_buffer{};
        type::DftBufferBuffer  m_dft_buffers{};
        type::DftAverageBuffer m_averaged_dft_buffers{};

        type::HistoryBuffer m_low_beat_buffer{};
        type::HistoryBuffer m_mid_beat_buffer{};
        type::HistoryBuffer m_high_beat_buffer{};

        BeatHandler m_low_beat_handler;
        BeatHandler m_mid_beat_handler;
        BeatHandler m_high_beat_handler;
    };

} // namespace app::audio

#endif // H_APP_AUDIO_SOUNDWINDOW_H
