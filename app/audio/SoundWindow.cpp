//
// Created by pc on 05-11-21.
//

#include "SoundWindow.h"

#include "AudioPlayer.h"
#include "Functions.h"

#include <fftw3.h>

namespace app::audio {

    SoundWindow::SoundWindow() = default;

    void SoundWindow::update(const audio::AudioPlayer& player) {
        const auto buffer = player.normalized_buffer();
        float      result = 0;
        for (size_t j = 0; j < AudioBuffer::size; j += 4)
            result += buffer[j];

        m_waveform.set(player.max_intensity() * result / (64.0f * std::numeric_limits<sf::Int16>::max()));
        auto                     complex_buffer = buffer.cast<Complex>(audio::float_to_complex());
        decltype(complex_buffer) out;
        fftw_plan                plan = fftw_plan_dft_1d(
            AudioBuffer::size, reinterpret_cast<fftw_complex*>(&complex_buffer.buffer()[0]), reinterpret_cast<fftw_complex*>(&out.buffer()[0]), FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(plan);
        fftw_destroy_plan(plan);

        m_dft_buffers.set({LinearBuffer(out.cast<float, DftBuffer::size>(audio::complex_to_dB()))});
        m_averaged_dft_buffers.set(m_dft_buffers.average());

        handle_energy();
    }

    const HistoryBuffer& SoundWindow::waveform() const {
        return m_waveform;
    }

    void SoundWindow::clear() {
        assert(false);
        //        m_waveform.clear();
        //        m_dft_buffers.clear();
        //        m_beat_buffer.clear();
    }

    const HistoryBuffer& SoundWindow::energy_buffer() const {
        return m_energy_buffer;
    }

    const HistoryBuffer& SoundWindow::energy_dif_buffer() const {
        return m_energy_dif_buffer;
    }

    void SoundWindow::handle_energy() {
        const auto energy = m_averaged_dft_buffers.last_set().energy();
        m_energy_buffer.set(energy);

        float average_energy = 0.0f;
        for (const auto& dft : m_averaged_dft_buffers.data())
            average_energy += dft.energy();

        average_energy /= audio::DftAverageBuffer::size;

        m_energy_dif_buffer.set((m_dft_buffers.look_back(0).energy() - m_dft_buffers.look_back(1).energy()) / average_energy);
        m_beat_handler.set_beat_score(energy_dif_buffer().last_set() / 3);
    }

    const DftAverageBuffer& SoundWindow::smoothed_dft_buffers() const {
        return m_averaged_dft_buffers;
    }

    float SoundWindow::current_beat_intensity() const {
        return m_beat_handler.was_just_blocked() ? m_beat_handler.intensity() : 0.0f;
    }

} // namespace app::audio