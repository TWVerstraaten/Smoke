//
// Created by pc on 05-11-21.
//

#include "SoundWindow.h"

#include "../math/Functions.h"
#include "../math/Math.h"
#include "AudioPlayer.h"

#include <fftw3.h>

namespace app::audio {

    SoundWindow::SoundWindow() = default;

    void SoundWindow::update_by_player(const audio::AudioPlayer& player) {
        static size_t offset = 0;

        size_t new_offset = player.current_offset();
        if (offset == new_offset)
            return;
        offset = new_offset;

        auto buffer = player.normalized_buffer();
        process_buffer(buffer);
    }

    double SoundWindow::energy_difference(size_t i_start, size_t i_end) {
        double difference = 0.0;
        for (size_t i = i_start; i != i_end; ++i)
            difference += std::max(0.0, m_averaged_dft_buffers.last_set()[i] - m_averaged_dft_buffers.look_back(1)[i]);

        return 100 * difference / m_averaged_dft_buffers.last_set().energy();
    }

    void SoundWindow::handle_energy([[maybe_unused]] const type::AudioBuffer& buffer) {
        static const size_t low_cut_off = 7;
        static const size_t mid_cut_off = 16;

        m_low_beat_handler.set_beat_score(energy_difference(0, low_cut_off));
        m_low_beat_buffer.set(energy_difference(0, low_cut_off));

        m_mid_beat_handler.set_beat_score(energy_difference(low_cut_off + 1, mid_cut_off));
        m_mid_beat_buffer.set(energy_difference(low_cut_off + 1, mid_cut_off));

        m_high_beat_handler.set_beat_score(energy_difference(mid_cut_off + 1, type::DftBuffer::s_size));
        m_high_beat_buffer.set(energy_difference(mid_cut_off + 1, type::DftBuffer::s_size));
    }

    void SoundWindow::set_waveform_value(const type::AudioBuffer& buffer) {
        double result = 0;
        for (size_t j = 0; j < type::AudioBuffer::s_size; j += type::AudioBuffer::s_size / 8)
            result += std::abs(buffer[j]);
        m_waveform.set(result * 8);
    }

    void SoundWindow::process_buffer(const type::AudioBuffer& buffer) {
        set_waveform_value(buffer);

        type::AudioBuffer smoothed_buffer = buffer;
        const size_t      n               = type::AudioBuffer::s_size;
        for (size_t i = 0; i != n; ++i)
            smoothed_buffer[i] *= 0.5 * (1.0 - std::cos((2.0 * M_PI * static_cast<double>(i)) / static_cast<double>(n)));

        auto                     complex_buffer = smoothed_buffer.cast<Complex>(math::double_to_complex());
        decltype(complex_buffer) out;
        fftw_plan                plan = fftw_plan_dft_1d(type::AudioBuffer::s_size,
                                          reinterpret_cast<fftw_complex*>(&complex_buffer.buffer()[0]),
                                          reinterpret_cast<fftw_complex*>(&out.buffer()[0]),
                                          FFTW_FORWARD,
                                          FFTW_ESTIMATE);
        fftw_execute(plan);
        fftw_destroy_plan(plan);

        auto new_dft = m_dft_buffers.last_set() / 1.08;
        auto t       = type::LinearBuffer(out.cast<double, type::DftBuffer::s_size>(math::complex_to_dB()));
        for (size_t i = 0; i != type::DftBuffer::s_size; ++i)
            new_dft[i] = std::max(new_dft[i], t[i]);

        m_dft_buffers.set(new_dft);
        m_averaged_dft_buffers.set(m_dft_buffers.average());
        handle_energy(smoothed_buffer);
    }

    const type::HistoryBuffer& SoundWindow::waveform() const {
        return m_waveform;
    }

    const type::DftAverageBuffer& SoundWindow::smoothed_dft_buffers() const {
        return m_averaged_dft_buffers;
    }

    float SoundWindow::current_beat_intensity() const {
        auto low  = m_low_beat_handler.current_beat_impuls();
        auto mid  = m_mid_beat_handler.current_beat_impuls();
        auto high = m_high_beat_handler.current_beat_impuls();
        return 0.5 * high;
        //        return std::sqrt(mid * mid + low * low + high * high);
    }

    const type::DftBufferBuffer& SoundWindow::dft_buffers() const {
        return m_dft_buffers;
    }

    const type::HistoryBuffer& SoundWindow::low_beat_buffer() const {
        return m_low_beat_buffer;
    }

    const type::HistoryBuffer& SoundWindow::mid_beat_buffer() const {
        return m_mid_beat_buffer;
    }

    const type::HistoryBuffer& SoundWindow::high_beat_buffer() const {
        return m_high_beat_buffer;
    }

} // namespace app::audio