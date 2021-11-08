//
// Created by pc on 07-11-21.
//

#ifndef H_APP_MAINWIDGET_H
#define H_APP_MAINWIDGET_H

#include "fl/Fluid.h"

#include <QBasicTimer>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QVector2D>

namespace app::disp {

    class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions {

      public:
        explicit MainWidget(QWidget* parent = nullptr);
        ~MainWidget() override;

      protected:
        void mousePressEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void keyPressEvent(QKeyEvent* e) override;

        void timerEvent(QTimerEvent* e) override;
        void initializeGL() override;
        void paintGL() override;
        void init_shaders();

      private:
        void init_buffers_and_vectors();

        int                  m_offset_uniform    = -1;
        float                m_offset            = 0.0f;
        size_t               m_vertical_points   = 600;
        size_t               m_horizontal_points = 600;
        fl::Fluid            m_fluid;
        std::vector<float>   m_vertices;
        std::vector<GLuint>  m_indices;
        QBasicTimer          m_timer;
        QOpenGLShaderProgram m_program;
        QOpenGLBuffer        m_array_buf;
        QOpenGLBuffer        m_index_buf;
        QVector2D            m_mouse_last_position;
        bool                 m_left_pressed  = false;
        bool                 m_right_pressed = false;
    };

} // namespace app::disp

#endif // H_APP_MAINWIDGET_H
