//
// Created by pc on 07-11-21.
//

#include "SmokeWidget.h"

#include "../tools/Profile.h"
#include "DisplaySettings.h"

#include <QKeyEvent>
#include <QPainter>
#include <cmath>

namespace app::disp {

    SmokeWidget::SmokeWidget(QWidget* parent) : QOpenGLWidget(parent) {
        m_elapsed_timer.start();
    }

    void SmokeWidget::initializeGL() {
        initializeOpenGLFunctions();
        QOpenGLFunctions::glEnable(GL_POINT_SIZE);
        glClearColor(0, 0, 0, 1);
        if ((not m_smoke_shader.init()) || (not m_line_shader.init())) {
            close();
        }
        m_smoke_renderer.init();
        m_line_renderer.init();
        m_timer.start(static_cast<int>(m_refresh_rate_target), this);
    }

    void SmokeWidget::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        {
            m_smoke_shader.bind();
            m_smoke_shader.set_uniforms();
            m_smoke_renderer.set_sample_points(width(), height());
            m_smoke_renderer.fill_quads(m_fluid);
            m_smoke_renderer.bind_quads();
            m_smoke_shader.set_locations();
            if (g_pixel_mode == PIXEL_MODE::PIXEL) {
                glPointSize(static_cast<float>(g_pixel_size));
                glDrawElements(GL_POINTS, static_cast<GLsizei>(m_smoke_renderer.index_count()), GL_UNSIGNED_INT, nullptr);
            } else {
                glPointSize(1);
                glDrawElements(GL_QUADS, static_cast<GLsizei>(m_smoke_renderer.index_count()), GL_UNSIGNED_INT, nullptr);
            }
            m_smoke_renderer.unbind();
            m_smoke_shader.unbind();
        }
        {
            m_line_shader.bind();
            m_line_shader.set_uniforms();
            std::vector<size_t> array;
            for (size_t i = 0; i != 121; ++i) {
                array.push_back(static_cast<size_t>(53.0f * (12.0f + std::sin(0.435 * static_cast<float>(i)))));
            }
            static float theta = 0.0f;
            m_line_renderer.fill_around_ellipse(array, 0.4, 0.8, theta, 0.8, 0.6);
            theta += 0.02;
            m_line_renderer.bind_lines();
            m_line_shader.set_locations();
            glDrawElements(GL_LINE_STRIP, static_cast<GLsizei>(m_line_renderer.index_count()), GL_UNSIGNED_INT, nullptr);
            m_line_renderer.unbind();
            m_line_shader.unbind();
        }
        PRINT_PROFILE();
        START_PROFILING();
    }

    void SmokeWidget::timerEvent(QTimerEvent* e) {
        Q_UNUSED(e)
        m_smoke_shader.update(static_cast<float>(m_elapsed_timer.elapsed()) / 1000.0f);
        m_line_shader.update(static_cast<float>(m_elapsed_timer.elapsed()) / 1000.0f);
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
        m_smoke_shader.set_circle();
    }

    void SmokeWidget::zoom() {
        m_smoke_shader.zoom();
        m_line_shader.zoom();
    }

} // namespace app::disp