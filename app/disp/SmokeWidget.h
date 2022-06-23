//
// Created by pc on 07-11-21.
//

#ifndef H_APP_DISP_SMOKEWIDGET_H
#define H_APP_DISP_SMOKEWIDGET_H

#include "AutoMover.h"
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

    class MainWindow;

    class SmokeWidget : public QOpenGLWidget, protected QOpenGLFunctions {

        Q_OBJECT

      public:
        explicit SmokeWidget(MainWindow* parent);
        ~SmokeWidget() override;

        void clear();
        void set_random_shape(double intensity);
        void zoom(double zoom_depth);
        void step(float elapsed);
        void handle_beat(float beat_score);
        void try_adding_manipulator(Manipulator&& manipulator);

      protected:
        void mousePressEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int w, int h) override;

      private:
        void draw_smoke();
        void draw_lines();
        void update_auto_movers(float elapsed);
        void remove_finished_manipulators();
        void update_manipulators(float elapsed);

        std::unique_ptr<fluid::Fluid> m_fluid;
        QElapsedTimer                 m_elapsed_timer;
        MouseState                    m_mouse_state;
        SmokeRenderer                 m_smoke_renderer;
        std::vector<AutoMover>        m_auto_movers;
        std::vector<Manipulator>      m_manipulators;
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKEWIDGET_H
