//
// Created by pc on 07-11-21.
//

#include "SmokeWidget.h"

#include "../tools/Profile.h"

#include <QKeyEvent>
#include <QPainter>
#include <iostream>

namespace app::disp {

    SmokeWidget::SmokeWidget(QWidget* parent) : QOpenGLWidget(parent) {
        m_elapsed_timer.start();
    }

    void SmokeWidget::initializeGL() {
        initializeOpenGLFunctions();
        glClearColor(0, 0, 0, 1);
        if (not m_shader.init()) {
            close();
        }
        m_drawing_buffers.init();
        m_timer.start(m_refresh_rate, this);
    }

    void SmokeWidget::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader.bind();
        m_shader.set_uniforms();
        m_drawing_buffers.fill_quads(m_fluid);
        m_drawing_buffers.bind_quads();
        m_shader.set_locations();
        glDrawElements(GL_QUADS, static_cast<GLsizei>(m_drawing_buffers.quad_count()), GL_UNSIGNED_INT, nullptr);
        m_drawing_buffers.unbind();
        m_shader.unbind();
        PRINT_PROFILE();
        START_PROFILING();

        //        QPainter p(this);
        //        p.setPen({Qt::red, 5});
        //        p.drawLine(QPointF{0, 0}, QPointF{220, 220});
    }

    void SmokeWidget::timerEvent(QTimerEvent* e) {
        Q_UNUSED(e)
        m_shader.update(static_cast<float>(m_elapsed_timer.elapsed()) / 1000.0f);
        m_elapsed_timer.restart();

        {
            PROFILE_NAMED("step");
            m_fluid.step();
            m_fluid.clear_previous();
        }
        update();
    }

    void SmokeWidget::mouseMoveEvent(QMouseEvent* e) {
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

    void SmokeWidget::mousePressEvent(QMouseEvent* e) {
        m_mouse_state.press(e->button());
        m_mouse_state.set_mouse_position(QVector2D(e->localPos()));
    }

    void SmokeWidget::mouseReleaseEvent(QMouseEvent* e) {
        m_mouse_state.release(e->button());
    }

    void SmokeWidget::clear() {
        m_fluid.clear_current();
        m_fluid.clear_previous();
    }

    void SmokeWidget::set_circle() {
        m_fluid.set_circle();
        m_shader.set_circle();
    }

    void SmokeWidget::zoom() {
        m_shader.zoom();
    }

} // namespace app::disp