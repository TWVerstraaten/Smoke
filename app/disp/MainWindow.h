//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_MAINWINDOW_H
#define H_APP_DISP_MAINWINDOW_H

#include "AudioWidget.h"

#include <QBasicTimer>
#include <QElapsedTimer>
#include <QMainWindow>

namespace app::audio {
    class AudioPlayer;
    class SoundWindow;
} // namespace app::audio

namespace app::disp {

    class SettingsWidget;
    class SmokeWidget;
    class AudioWidget;

    class MainWindow final : public QMainWindow {
        Q_OBJECT

        friend class AudioWidget;
        friend class SmokeWidget;

      public:
        MainWindow();
        ~MainWindow() final;

        void keyPressEvent(QKeyEvent* e) override;

      protected:
        void closeEvent(QCloseEvent* event) override;
        void timerEvent(QTimerEvent* e) override;

      private:
        // These will be owned by Qt frame work
        SmokeWidget*    m_smoke_widget;
        SettingsWidget* m_settings_widget;

        // The following need to be in this order because m_audio_widget relies on m_sound_window and m_player
        std::unique_ptr<audio::SoundWindow> m_sound_window;
        std::unique_ptr<audio::AudioPlayer> m_player;
        std::unique_ptr<AudioWidget>        m_audio_widget;

        QBasicTimer   m_timer;
        QElapsedTimer m_elapsed_timer;
        QElapsedTimer m_follower;
    };

} // namespace app::disp

#endif // H_APP_DISP_MAINWINDOW_H
