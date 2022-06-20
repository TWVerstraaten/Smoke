//
// Created by pc on 07-11-21.
//

#ifndef H_APP_DISP_SMOKEWIDGET_H
#define H_APP_DISP_SMOKEWIDGET_H

#include "AutoMover.h"
#include "LineRenderer.h"
#include "ManipulatorBase.h"
#include "MouseState.h"
#include "SmokeRenderer.h"

#include <QBasicTimer>
#include <QElapsedTimer>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <vector>

namespace app::fluid {
    class Fluid;
}

namespace app::disp {

    class SmokeWidget : public QOpenGLWidget, protected QOpenGLFunctions {

        Q_OBJECT

      public:
        explicit SmokeWidget(QWidget* parent = nullptr);
        ~SmokeWidget() override;

        void clear();
        void set_random_shape();
        void zoom(float zoom_depth);
        void step(float elapsed);
        void handle_beat(float beat_score);

      protected:
        void mousePressEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void initializeGL() override;
        void paintGL() override;

      private:
        void draw_smoke();
        void draw_lines();
        void try_adding_manipulator(Manipulator&& manipulator);

        std::unique_ptr<fluid::Fluid> m_fluid;
        MouseState                    m_mouse_state;
        LineRenderer                  m_line_renderer;
        SmokeRenderer                 m_smoke_renderer;
        std::vector<AutoMover>        m_auto_movers;
        std::vector<Manipulator>      m_manipulators;

        struct Shot {
            std::function<void(float)> m_func;
            float                      m_probability;
        };

        std::vector<Shot> m_shots;
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKEWIDGET_H
