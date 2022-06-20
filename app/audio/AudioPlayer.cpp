//
// Created by pc on 05-11-21.
//

#include "AudioPlayer.h"

#include "LinearBuffer.h"

namespace app::audio {

    void AudioPlayer::init(const std::string& path_to_file) {
        m_sound_buffer.loadFromFile(path_to_file);

        const auto total_sample_count = m_sound_buffer.getSampleCount();
        auto*      samples            = m_sound_buffer.getSamples();
        unsigned   max                = 0;
        for (size_t i = 0; i != total_sample_count; ++i) {
            const auto current = std::abs(*(samples + i));
            if (current > static_cast<int>(max))
                max = current;
        }
        m_max_intensity = max;
        m_sound.setBuffer(m_sound_buffer);
        m_samples_rate = m_sound_buffer.getSampleRate() * m_sound_buffer.getChannelCount();
        m_sound.setLoop(true);
        m_clock.restart();
    }

    size_t AudioPlayer::current_offset() const {
        size_t offset = static_cast<size_t>(m_sound.getPlayingOffset().asSeconds() * static_cast<float>(m_samples_rate));
        if (offset != m_last_sfml_offset) {
            m_last_sfml_offset = offset;
            m_clock.restart();
            return m_last_sfml_offset;
        }
        return static_cast<size_t>((m_sound.getPlayingOffset().asSeconds() + m_clock.getElapsedTime().asSeconds()) * static_cast<float>(m_samples_rate));
    }

    void AudioPlayer::play() {
        if (m_is_paused) {
            m_sound.play();
            m_is_paused = false;
        }
    }

    void AudioPlayer::pause() {
        if (not m_is_paused) {
            m_sound.pause();
            m_is_paused = true;
        }
    }

    void AudioPlayer::set_paused(bool should_pause) {
        if (should_pause)
            pause();
        else
            play();
    }

    AudioBuffer AudioPlayer::normalized_buffer() const {
        const size_t offset             = current_offset();
        const size_t total_sample_count = m_sound_buffer.getSampleCount();
        if (offset >= total_sample_count)
            return AudioBuffer{};

        return AudioBuffer(static_cast<const sf::Int16*>(m_sound_buffer.getSamples() + offset),
                           std::function<float(sf::Int16)>([](sf::Int16 s) { return static_cast<float>(s) / std::numeric_limits<sf::Int16>::max(); }),
                           std::min(total_sample_count - offset, AudioBuffer::size));
    }

    bool AudioPlayer::is_paused() const {
        return m_is_paused;
    }

    unsigned int AudioPlayer::max_intensity() const {
        return m_max_intensity;
    }

} // namespace app::audio