//
// Created by pc on 08-11-21.
//

#include "MainWindow.h"

#include "../../fluid/FluidSettings.h"
#include "../State.h"
#include "../audio/AudioPlayer.h"
#include "../audio/SoundWindow.h"
#include "../thr/ThreadPool.h"
#include "../thr/ThreadSettings.h"
#include "AudioWidget.h"
#include "BeatWidget.h"
#include "DispSettings.h"
#include "SettingsWidget.h"
#include "SmokeWidget.h"

#include <QKeyEvent>

namespace app::disp {

    MainWindow::MainWindow()
        : m_smoke_widget(new SmokeWidget(this)),
          m_settings_widget(new SettingsWidget()),
          m_sound_window(new audio::SoundWindow()),
          m_player(new audio::AudioPlayer()),
          m_audio_widget(new AudioWidget(this)) {
        setCentralWidget(m_smoke_widget);
        m_audio_widget->show();

        m_settings_widget->add_section("Simulation");
        m_settings_widget->add("Force", app::fluid::g_force_input, 0.01f * app::fluid::g_force_input, 100 * app::fluid::g_force_input, false);
        m_settings_widget->add("Particles", app::fluid::g_particle_input, 0.01f * app::fluid::g_particle_input, 100 * app::fluid::g_particle_input, false);
        m_settings_widget->add("Visc", app::fluid::g_viscosity_coefficient, 0.01f * app::fluid::g_viscosity_coefficient, 100 * app::fluid::g_viscosity_coefficient, false);
        m_settings_widget->add("Diff", app::fluid::g_diffusion_coefficient, 0.01f * app::fluid::g_diffusion_coefficient, 100 * app::fluid::g_diffusion_coefficient, false);
        m_settings_widget->add("Time update_widgets", app::fluid::g_dt, 0.001, 0.025, true);

        //        m_settings_widget->add("Zoom Depth", g_zoom_depth);
        //        m_settings_widget->add("Zoom Decay", g_zoom_decay);
        m_settings_widget->add_section("Performance");
        m_settings_widget->add("Multi thread", thr::g_multi_thread);
        m_settings_widget->add("Thread count", thr::g_thread_count, 1, 32, [](size_t v) { thr::ThreadPool::get().resize(v); });

        m_settings_widget->add_section("Display");
        //        m_settings_widget->add("Power scale", g_power_scale, 0.1f, 2.0f, false);
        m_settings_widget->add("Clamp counts", g_clamp_count, 2);
        m_settings_widget->add("Clamp coef", g_clamp_coefficient, 0.0, 1.0, true);
        m_settings_widget->add("Invert colors", g_invert_colors);
        m_settings_widget->add("Color mode",
                               g_color_mode,
                               {{"gray", COLOR_MODE::GRAY},
                                {"hsl", COLOR_MODE::HSL},
                                {"hsv", COLOR_MODE::HSV},
                                {"water", COLOR_MODE::WATER},
                                {"test1", COLOR_MODE::TEST_1},
                                {"test2", COLOR_MODE::TEST_2}});
        m_settings_widget->add("Pixel s_size", g_pixel_size, 1);

        m_settings_widget->add_section("Shots");
        m_settings_widget->add("Circle", [this]() { m_smoke_widget->set_random_shape(0); });
        m_settings_widget->add("Zoom", [this]() { m_smoke_widget->zoom(g_zoom_depth); });
        m_settings_widget->add("Boom", [this]() {
            m_smoke_widget->zoom(g_zoom_depth);
            m_smoke_widget->set_random_shape(0);
        });

        m_settings_widget->show();
        resize(1000, 1000);

        //        m_player->init("../songs/acid sweep_23.wav");
        //        m_player->init("../songs/sean paul_2.wav");
        //        m_player->init("../songs/minimal.wav");
        //        m_player->init("../songs/min.wav");
//        m_player->init("../songs/636924__djgriffin__my-80-s-broken-synth.wav");
        m_player->init("../songs/608412__djgriffin__grand-piano-c-scale-alt.wav");
        //                m_player->init("../songs/251284__djgriffin__135-ravey-game-loop-6.wav");
        //        m_player->init("../songs/593784__djgriffin__gleam-machine-retro-video-game-loop-with-beat.wav");
        //        m_player->init("../songs/592237__djgriffin__18th-521-loop.wav");
        //                m_player->init("../songs/beat1.wav");
        m_player->play();
        m_elapsed_timer.start();
        m_follower.start();
        m_timer.start(5, this);
    }

    MainWindow::~MainWindow() = default;

    void MainWindow::keyPressEvent(QKeyEvent* e) {
        switch (e->key()) {
            case Qt::Key::Key_Space:
                State::get().toggle_paused();
                m_player->set_paused(State::get().is_paused());
                break;
            case Qt::Key::Key_Escape:
                m_smoke_widget->clear();
                break;
            default:
                break;
        }
    }

    void MainWindow::closeEvent(QCloseEvent* event) {
        m_settings_widget->close();
        QWidget::closeEvent(event);
    }

    void MainWindow::timerEvent(QTimerEvent* e) {
        Q_UNUSED(e)
        float elapsed = static_cast<float>(m_elapsed_timer.elapsed()) / 1000.0f;
        qDebug() << "Frama rate = " << 1.0f / elapsed;
        m_elapsed_timer.restart();

        if (State::get().is_paused())
            return;

        m_smoke_widget->handle_beat(m_sound_window->current_beat_intensity());
        m_smoke_widget->step(elapsed);
        m_sound_window->update_by_player(*m_player);
        m_audio_widget->update_widgets();

        //        static size_t i      = 0;
        //        QPixmap       pixmap = m_smoke_widget->grab();
        //        pixmap.save("mapje/fr" + QString::number(i) + ".png");
        //        ++i;
    }

} // namespace app::disp