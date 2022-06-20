#ifndef H_APP_DISP_AUDIOWIDGET_H
#define H_APP_DISP_AUDIOWIDGET_H

#include "../audio/AudioPlayer.h"
#include "../audio/SoundWindow.h"

#include <QBasicTimer>
#include <QFormLayout>
#include <QWidget>

namespace app::disp {

    class MainWindow;
    class BeatWidget;
    class HistoryWidget;
    class SpectrumWidget;

    class AudioWidget : public QWidget {
        Q_OBJECT

      public:
        explicit AudioWidget(MainWindow* parent);

        void update_widgets();

      private:
        audio::SoundWindow* m_sound_window;
        audio::AudioPlayer* m_player;
        SpectrumWidget*     m_spectrum_widget;
        HistoryWidget*      m_wave_form_widget;
        HistoryWidget*      m_energy_widget;
        HistoryWidget*      m_energy_dif_widget;
        BeatWidget*         m_beat_widget;
        QFormLayout*        m_layout;
    };

} // namespace app::disp

#endif // H_APP_DISP_AUDIOWIDGET_H
