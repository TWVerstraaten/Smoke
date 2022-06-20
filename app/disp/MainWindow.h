//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_MAINWINDOW_H
#define H_APP_DISP_MAINWINDOW_H

#include "AudioWidget.h"

#include <QBasicTimer>
#include <QElapsedTimer>
#include <QMainWindow>

namespace app::disp {

    class SettingsWidget;
    class SmokeWidget;
    class AudioWidget;

    class MainWindow : public QMainWindow {
        Q_OBJECT

        friend class AudioWidget;

      public:
        MainWindow();

        void keyPressEvent(QKeyEvent* e) override;

      protected:
        void closeEvent(QCloseEvent* event) override;
        void timerEvent(QTimerEvent* e) override;

      private:
        SmokeWidget*    m_smoke_widget;
        SettingsWidget* m_settings_widget;

        audio::SoundWindow m_sound_window;
        audio::AudioPlayer m_player;

        std::unique_ptr<AudioWidget> m_audio_widget;

        QBasicTimer   m_timer;
        QElapsedTimer m_elapsed_timer;
    };

} // namespace app::disp

#endif // H_APP_DISP_MAINWINDOW_H
