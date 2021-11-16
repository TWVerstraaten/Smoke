//
// Created by pc on 07-11-21.
//

#include "MainWidget.h"

#include <QKeyEvent>

namespace app::disp {

    MainWidget::MainWidget(QWidget* parent) : QOpenGLWidget(parent) {
    }

    void MainWidget::initializeGL() {
        initializeOpenGLFunctions();
        glClearColor(0, 0, 0, 1);
        if (not m_shader.init()) {
            close();
        }
        m_drawing_buffers.init();
        m_timer.start(6, this);
        m_shader.set_uniform_screen_size(width(), height());
        m_open_gl_initialized = true;
    }

    void MainWidget::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader.update(0.16);
        m_shader.set_locations();

        glDrawElements(GL_QUADS, m_drawing_buffers.quad_count(), GL_UNSIGNED_INT, nullptr);
    }

    void MainWidget::timerEvent(QTimerEvent* e) {
        Q_UNUSED(e);
        //                m_fluid.add_bar();
        m_fluid.step(0.005);
        m_drawing_buffers.fill_and_bind(m_fluid);
        m_fluid.clear_previous();
        update();
    }

    void MainWidget::mouseMoveEvent(QMouseEvent* e) {
        if (not m_mouse_state.left_or_right_pressed()) {
            return;
        }
        QVector2D   direction = m_mouse_state.set_mouse_position(QVector2D(e->localPos()));
        const float width     = static_cast<float>(size().width());
        const float height    = static_cast<float>(size().height());
        direction[0] /= width;
        direction[1] /= height;
        const auto new_mouse_position = m_mouse_state.mouse_position();

        if (m_mouse_state.left_pressed()) {
            m_fluid.add_density(new_mouse_position.x() / width, 1.0f - new_mouse_position.y() / height);
        } else if (m_mouse_state.right_pressed()) {
            m_fluid.add_density(new_mouse_position.x() / width, 1.0f - new_mouse_position.y() / height, -1.0f);
        }
        m_fluid.add_velocity(new_mouse_position.x() / width, 1.0f - new_mouse_position.y() / height, direction.x(), -direction.y());
    }

    void MainWidget::mousePressEvent(QMouseEvent* e) {
        m_mouse_state.press(e->button());
        m_mouse_state.set_mouse_position(QVector2D(e->localPos()));
    }

    void MainWidget::mouseReleaseEvent(QMouseEvent* e) {
        m_mouse_state.release(e->button());
    }

    void MainWidget::clear() {
        m_fluid.clear_current();
        m_fluid.clear_previous();
    }

    void MainWidget::set_circle() {
        m_fluid.set_circle();
        m_shader.set_circle();
    }

    void MainWidget::zoom() {
        m_shader.zoom();
    }

    void MainWidget::resizeEvent(QResizeEvent* e) {
        if (m_open_gl_initialized) {
            m_shader.set_uniform_screen_size(width(), height());
        }
        QOpenGLWidget::resizeEvent(e);
    }

} // namespace app::disp