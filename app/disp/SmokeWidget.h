//
// Created by pc on 07-11-21.
//

#ifndef H_APP_DISP_SMOKEWIDGET_H
#define H_APP_DISP_SMOKEWIDGET_H

#include "../fl/Fluid.h"
#include "LineRenderer.h"
#include "LineShader.h"
#include "MouseState.h"
#include "SmokeRenderer.h"
#include "SmokeShader.h"

#include <QBasicTimer>
#include <QElapsedTimer>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QVector2D>

namespace app::disp {

    class SmokeWidget : public QOpenGLWidget, protected QOpenGLFunctions {

        Q_OBJECT

      public:
        explicit SmokeWidget(QWidget* parent = nullptr);

        void clear();
        void set_circle();
        void zoom();

      protected:
        void mousePressEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void timerEvent(QTimerEvent* e) override;
        void initializeGL() override;
        void paintGL() override;

      private:
        MouseState    m_mouse_state;
        LineShader    m_line_shader;
        LineRenderer  m_line_renderer;
        SmokeShader   m_smoke_shader;
        SmokeRenderer m_smoke_renderer;
        fl::Fluid     m_fluid;
        QElapsedTimer m_elapsed_timer;
        QBasicTimer   m_timer;
        size_t        m_refresh_rate_target = 10;
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKEWIDGET_H
