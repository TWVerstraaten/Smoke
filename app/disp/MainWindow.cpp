//
// Created by pc on 08-11-21.
//

#include "MainWindow.h"

#include "../fl/Settings.h"
#include "MainWidget.h"
#include "SettingsWidget.h"
#include "ShaderSettings.h"

#include <QKeyEvent>

namespace app::disp {

    MainWindow::MainWindow() : m_main_widget(new MainWidget(this)), m_settings_widget(new SettingsWidget()) {
        setCentralWidget(m_main_widget);

        //        m_settings_widget->add("Force", app::fl::g_force_input);
        //        m_settings_widget->add("Particles", app::fl::g_particle_input);
        //        m_settings_widget->add("Visc", app::fl::g_viscosity_coefficient);
        //        m_settings_widget->add("Diff", app::fl::g_diffusion_coefficient);
        //        m_settings_widget->add("Zoom Depth", g_zoom_depth);
        //        m_settings_widget->add("Zoom Decay", g_zoom_decay);
        m_settings_widget->add("Multi thread", fl::g_multi_thread);
        m_settings_widget->add("Clamp colors", g_clamp_colors);
        m_settings_widget->add("Clamp counts", g_clamp_count);
        m_settings_widget->add("Circle", [this]() { m_main_widget->set_circle(); });
        m_settings_widget->add("Zoom", [this]() { m_main_widget->zoom(); });
        m_settings_widget->add("Boom", [this]() {
            m_main_widget->zoom();
            m_main_widget->set_circle();
        });
        m_settings_widget->add("Invert colors", g_invert_colors);
        m_settings_widget->add("Color mode", g_color_mode, {{"gray", COLOR_MODE::GRAY}, {"rgb", COLOR_MODE::RGB}, {"space", COLOR_MODE::SPACE}});

        m_settings_widget->show();
        resize(1000, 1000);
    }

    void MainWindow::keyPressEvent(QKeyEvent* e) {
        switch (e->key()) {
            case Qt::Key::Key_Space:
                m_main_widget->clear();
                break;
            case Qt::Key::Key_Escape:
                close();
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