//
// Created by pc on 20-6-22.
//

#include "BeatHandler.h"

#include <QTimer>

namespace app::audio {

    void BeatHandler::set_beat_score(float beat_score) {
        if (m_was_just_blocked)
            m_was_just_blocked = false;
        if (not(m_is_blocked))
            if (beat_score > m_beat_cut_off) {
                m_intensity = beat_score;
                //                m_intensity  = std::min(0.99f, beat_score);
                m_is_blocked       = true;
                m_was_just_blocked = true;
                QTimer::singleShot(m_beat_cooldown_msec, [this] { m_is_blocked = false; });
            }
    }

    float BeatHandler::intensity() const {
        return m_intensity;
    }

    bool BeatHandler::was_just_blocked() const {
        return m_was_just_blocked;
    }

} // namespace app::audio