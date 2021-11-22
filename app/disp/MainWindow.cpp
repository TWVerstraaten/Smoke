//
// Created by pc on 08-11-21.
//

#include "MainWindow.h"

#include "../State.h"
#include "../fluid/FlSettings.h"
#include "../tools/ThreadPool.h"
#include "../tools/ThreadSettings.h"
#include "DispSettings.h"
#include "SettingsWidget.h"
#include "SmokeWidget.h"

#include <QKeyEvent>

namespace app::disp {

    MainWindow::MainWindow() : m_smoke_widget(new SmokeWidget(this)), m_settings_widget(new SettingsWidget()) {
        setCentralWidget(m_smoke_widget);

        m_settings_widget->add_section("Simulation");
        m_settings_widget->add("Force", app::fluid::g_force_input);
        m_settings_widget->add("Particles", app::fluid::g_particle_input);
        m_settings_widget->add("Visc", app::fluid::g_viscosity_coefficient);
        m_settings_widget->add("Diff", app::fluid::g_diffusion_coefficient);
        m_settings_widget->add("Time step", app::fluid::g_dt, 0.001, 2.0);

        //        m_settings_widget->add("Zoom Depth", g_zoom_depth);
        //        m_settings_widget->add("Zoom Decay", g_zoom_decay);
        m_settings_widget->add_section("Performance");
        m_settings_widget->add("Multi thread", tools::g_multi_thread);
        m_settings_widget->add("Thread count", tools::g_thread_count, 1, 32, [](size_t v) { tools::ThreadPool::get().resize(v); });

        m_settings_widget->add_section("Display");
        m_settings_widget->add("Power scale", g_power_scale, 0.1f, 1.0f, true);
        m_settings_widget->add("Clamp counts", g_clamp_count, 2);
        m_settings_widget->add("Clamp coef", g_clamp_coefficient, 0.0, 1.0, true);
        m_settings_widget->add("Invert colors", g_invert_colors);
        m_settings_widget->add("Color mode", g_color_mode, {{"gray", COLOR_MODE::GRAY}, {"rgb", COLOR_MODE::RGB}, {"space", COLOR_MODE::SPACE}});
        m_settings_widget->add("Draw mode", g_pixel_mode, {{"normal", PIXEL_MODE::NORMAL}, {"pixel", PIXEL_MODE::PIXEL}});
        m_settings_widget->add("Pixel size", g_pixel_size, 1);

        m_settings_widget->add_section("Shots");
        m_settings_widget->add("Circle", [this]() { m_smoke_widget->set_circle(); });
        m_settings_widget->add("Zoom", [this]() { m_smoke_widget->zoom(); });
        m_settings_widget->add("Boom", [this]() {
            m_smoke_widget->zoom();
            m_smoke_widget->set_circle();
        });

        m_settings_widget->show();
        resize(1000, 1000);
    }

    void MainWindow::keyPressEvent(QKeyEvent* e) {
        switch (e->key()) {
            case Qt::Key::Key_Space:
                State::get().toggle_paused();
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

} // namespace app::disp