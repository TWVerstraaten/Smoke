//
// Created by pc on 05-12-21.
//

#ifndef H_SRC_DISP_BEATWIDGET_H
#define H_SRC_DISP_BEATWIDGET_H

#include <QWidget>

namespace app::disp {

    class BeatWidget : public QWidget {
        Q_OBJECT

      public:
        explicit BeatWidget(QWidget* parent);

        //        void                set_beat_score(float beat_score);
        //        [[nodiscard]] float intensity() const;
        //        [[nodiscard]] bool  was_just_blocked() const;

        void set_intensity(float intensity);

      protected:
        void paintEvent(QPaintEvent* event) override;

      private:
        //        bool   m_is_blocked         = false;
        //        bool   m_was_just_blocked   = false;
        float m_intensity = 0.0f;
        //        float  m_beat_cut_off       = 0.34f;
        //        size_t m_beat_cooldown_msec = 150;
    };

} // namespace app::disp

#endif // H_SRC_DISP_BEATWIDGET_H
