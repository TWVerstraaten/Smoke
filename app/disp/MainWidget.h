//
// Created by pc on 07-11-21.
//

#ifndef H_APP_DISP_MAINWIDGET_H
#define H_APP_DISP_MAINWIDGET_H

#include "../fl/Fluid.h"
#include "DrawingBuffers.h"
#include "MouseState.h"
#include "Shader.h"

#include <QBasicTimer>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QVector2D>

namespace app::disp {

    class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions {

        Q_OBJECT

      public:
        explicit MainWidget(QWidget* parent = nullptr);

        void clear();
        void set_circle();
        void zoom();

      protected:
        void mousePressEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void resizeEvent(QResizeEvent* e) override;
        void timerEvent(QTimerEvent* e) override;
        void initializeGL() override;
        void paintGL() override;

      private:
        DrawingBuffers m_drawing_buffers;
        Shader         m_shader;
        fl::Fluid      m_fluid;
        QBasicTimer    m_timer;
        MouseState     m_mouse_state;
        bool           m_open_gl_initialized = false;
    };

} // namespace app::disp

#endif // H_APP_DISP_MAINWIDGET_H
