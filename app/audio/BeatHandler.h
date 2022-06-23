//
// Created by pc on 20-6-22.
//

#ifndef H_APP_AUDIO_BEATHANDLER_H
#define H_APP_AUDIO_BEATHANDLER_H

#include <cstddef>

namespace app::audio {

    class BeatHandler {

      public:
        void                 set_beat_score(double beat_score);
        [[nodiscard]] double intensity() const;
        [[nodiscard]] bool   was_just_blocked() const;
        [[nodiscard]] double current_beat_impuls() const;

      private:
        bool   m_is_blocked         = false;
        bool   m_was_just_blocked   = false;
        double m_intensity          = 0.0f;
        double m_beat_cut_off       = 0.3f;
        size_t m_beat_cooldown_msec = 220;
        bool   m_beat_is_coming     = false;
        double m_running_average    = 0.0f;
        size_t m_force_beat_wait    = 10;
    };

} // namespace app::audio

#endif // H_APP_AUDIO_BEATHANDLER_H
