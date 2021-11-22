//
// Created by pc on 07-11-21.
//

#ifndef H_APP_DISP_SMOKEWIDGET_H
#define H_APP_DISP_SMOKEWIDGET_H

#include "LineRenderer.h"
#include "MouseState.h"
#include "SmokeRenderer.h"

#include <QBasicTimer>
#include <QElapsedTimer>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

namespace app::disp {

    class SmokeWidget : public QOpenGLWidget, protected QOpenGLFunctions {

        Q_OBJECT

      public:
        explicit SmokeWidget(QWidget* parent = nullptr);
        ~SmokeWidget() override;

        void clear();
        void set_circle();

        static void zoom();

      protected:
        void mousePressEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void timerEvent(QTimerEvent* e) override;
        void initializeGL() override;
        void paintGL() override;

      private:
        void draw_smoke();
        void draw_lines();

        std::unique_ptr<fluid::Fluid> m_fluid;
        const size_t               m_refresh_rate_target = 10;
        QBasicTimer                m_timer;
        QElapsedTimer              m_elapsed_timer;
        MouseState                 m_mouse_state;
        LineRenderer               m_line_renderer;
        SmokeRenderer              m_smoke_renderer;
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKEWIDGET_H
