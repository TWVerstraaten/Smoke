//
// Created by pc on 05-11-21.
//

#ifndef H_APP_AUDIO_AUDIOPLAYER_H
#define H_APP_AUDIO_AUDIOPLAYER_H

#include "../type/BufferTypes.h"

#include <SFML/Audio.hpp>
#include <cassert>
#include <string>

namespace app::audio {

    class AudioPlayer {

      public:
        explicit AudioPlayer() = default;

        void                            init(const std::string& path_to_file);
        void                            play();
        void                            pause();
        void                            set_paused(bool should_pause);
        [[nodiscard]] bool              is_paused() const;
        [[nodiscard]] unsigned int      max_intensity() const;
        [[nodiscard]] size_t            current_offset() const;
        [[nodiscard]] type::AudioBuffer normalized_buffer() const;

      private:
        bool              m_is_paused     = true;
        unsigned          m_max_intensity = 0;
        size_t            m_samples_rate  = 0;
        sf::Sound         m_sound{};
        sf::SoundBuffer   m_sound_buffer{};
        mutable size_t    m_last_sfml_offset = 0;
    };

} // namespace app::audio

#endif // H_APP_AUDIO_AUDIOPLAYER_H
