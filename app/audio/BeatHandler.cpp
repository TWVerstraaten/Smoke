//
// Created by pc on 20-6-22.
//

#include "BeatHandler.h"

#include <QTimer>

namespace app::audio {

    void BeatHandler::set_beat_score(double beat_score) {
        if (m_was_just_blocked)
            m_was_just_blocked = false;
        if (not(m_is_blocked)) {
            if (m_beat_is_coming) {
                m_running_average = 0.5 * m_running_average + 0.5 * beat_score;
                if ((2 * m_running_average < m_intensity) || (m_force_beat_wait == 0)) {
                    m_is_blocked       = true;
                    m_was_just_blocked = true;
                    m_beat_is_coming   = false;
                    m_running_average  = 0.0f;
                    QTimer::singleShot(m_beat_cooldown_msec, [this] { m_is_blocked = false; });
                }
                --m_force_beat_wait;
            } else if (beat_score > m_beat_cut_off) {
                m_intensity       = beat_score;
                m_beat_is_coming  = true;
                m_running_average = 0.0f;
                m_force_beat_wait = 10;
                QTimer::singleShot(m_beat_cooldown_msec, [this] { m_is_blocked = false; });
            }
        }
    }

    double BeatHandler::intensity() const {
        return m_intensity;
    }

    bool BeatHandler::was_just_blocked() const {
        return m_was_just_blocked;
    }

    double BeatHandler::current_beat_impuls() const {
        return was_just_blocked() ? intensity() : 0.0f;
    }

} // namespace app::audio