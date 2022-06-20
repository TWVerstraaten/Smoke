//
// Created by pc on 20-6-22.
//

#ifndef H_APP_AUDIO_BEATHANDLER_H
#define H_APP_AUDIO_BEATHANDLER_H

#include <QObject>
#include <cstddef>

namespace app::audio {

    class BeatHandler : public  QObject{

        Q_OBJECT

      public:
        void                set_beat_score(float beat_score);
        [[nodiscard]] float intensity() const;
        [[nodiscard]] bool  was_just_blocked() const;

      private:
        bool   m_is_blocked         = false;
        bool   m_was_just_blocked   = false;
        float  m_intensity          = 0.0f;
        float  m_beat_cut_off       = 0.34f;
        size_t m_beat_cooldown_msec = 150;
    };

} // namespace app::audio

#endif // H_APP_AUDIO_BEATHANDLER_H
