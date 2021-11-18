//
// Created by pc on 07-11-21.
//

#ifndef H_APP_DISP_SMOKEWIDGET_H
#define H_APP_DISP_SMOKEWIDGET_H

#include "../fl/Fluid.h"
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
        QBasicTimer   m_timer;
        QElapsedTimer m_elapsed_timer;
        MouseState    m_mouse_state;
        SmokeShader   m_shader;
        SmokeRenderer m_drawing_buffers;
        fl::Fluid     m_fluid;
        size_t        m_refresh_rate = 10;
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKEWIDGET_H
