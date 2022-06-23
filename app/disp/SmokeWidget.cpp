//
// Created by pc on 07-11-21.
//

#include "SmokeWidget.h"

#include "../../fluid/Fluid.h"
#include "../State.h"
#include "../math/Math.h"
#include "../prf/Profile.h"
#include "DispSettings.h"
#include "MainWindow.h"
#include "SmokeEffect.h"

#include <QMouseEvent>

namespace app::disp {

    SmokeWidget::SmokeWidget(MainWindow* parent) : QOpenGLWidget(parent), m_fluid(new fluid::Fluid) {
        //        m_auto_movers.resize(2);
        m_elapsed_timer.start();
    }

    SmokeWidget::~SmokeWidget() = default;

    void SmokeWidget::initializeGL() {
        initializeOpenGLFunctions();
        QOpenGLFunctions::glEnable(GL_POINT_SIZE);
        glClearColor(0, 0, 0, 1);
        if ((not m_smoke_renderer.init()))
            close();
    }

    void SmokeWidget::paintGL() {
        //        PROFILE_FUNCTION();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw_smoke();
        draw_lines();
        //        PRINT_PROFILE();
        //        START_PROFILING();
    }

    void SmokeWidget::step(float elapsed) {
        //        PROFILE_FUNCTION();
        if (not State::get().is_paused()) {
            ShaderBase::update_statics(elapsed);

            update_auto_movers(elapsed);
            remove_finished_manipulators();
            update_manipulators(elapsed);

            m_fluid->step();
        }
        update();
    }

    void SmokeWidget::remove_finished_manipulators() {
        static const auto is_done_lambda = [](const auto& v) { return std::visit([](const auto& obj) { return obj.is_done(); }, v); };
        m_manipulators.erase(std::remove_if(m_manipulators.begin(), m_manipulators.end(), is_done_lambda), m_manipulators.end());
    }

    void SmokeWidget::update_manipulators(float elapsed) {
        for (auto& manipulator : m_manipulators)
            std::visit([elapsed](auto& a) { a.update(elapsed); }, manipulator);
    }

    void SmokeWidget::update_auto_movers(float elapsed) {
        for (auto& auto_mover : m_auto_movers) {
            auto_mover.update(elapsed);
            m_fluid->add_density(auto_mover.position(), 0.5);
            m_fluid->add_velocity(auto_mover.position(), 0.09 * auto_mover.velocity());
        }
    }

    void SmokeWidget::mouseMoveEvent(QMouseEvent* e) {
        if (not m_mouse_state.any_pressed())
            return;

        const float width              = static_cast<float>(size().width());
        const float height             = static_cast<float>(size().height());
        const auto  new_mouse_position = QPointF(e->localPos());

        auto direction = new_mouse_position - m_mouse_state.mouse_position();
        direction.setX(direction.x() / width);
        direction.setY(direction.y() / height);

        if (m_mouse_state.left_pressed())
            m_fluid->add_density(new_mouse_position.x() / width, 1.0f - new_mouse_position.y() / height);
        else if (m_mouse_state.right_pressed())
            m_fluid->add_density(new_mouse_position.x() / width, 1.0f - new_mouse_position.y() / height, -1.0f);

        m_fluid->add_velocity(new_mouse_position.x() / width, 1.0f - new_mouse_position.y() / height, direction.x(), -direction.y());
        m_mouse_state.set_mouse_position(new_mouse_position);
    }

    void SmokeWidget::mousePressEvent(QMouseEvent* e) {
        m_mouse_state.press(e->button());
        m_mouse_state.set_mouse_position(QPointF(e->localPos()));
    }

    void SmokeWidget::mouseReleaseEvent(QMouseEvent* e) {
        m_mouse_state.release(e->button());
    }

    void SmokeWidget::clear() {
        m_fluid->clear_current();
        m_fluid->clear_previous();
    }

    void SmokeWidget::set_random_shape(double intensity) {
        m_fluid->set_polygon(3 + math::random_number_in_range(0, 8), app::math::clamp(intensity, 0.0, 1.0), app::math::Point(0.5, 0.5));
    }

    void SmokeWidget::zoom(double zoom_depth) {
        ShaderBase::zoom(zoom_depth);
    }

    void SmokeWidget::draw_smoke() {
        m_smoke_renderer.set_sample_points(width(), height());
        m_smoke_renderer.fill(*m_fluid);
        m_smoke_renderer.bind();
        glPointSize(static_cast<float>(g_pixel_size));
        glDrawElements(GL_POINTS, static_cast<GLsizei>(m_smoke_renderer.index_count()), GL_UNSIGNED_INT, nullptr);
        m_smoke_renderer.unbind();
    }

    void SmokeWidget::draw_lines() {
        //        m_line_renderer.fill_around_circle(m_sound_window->smoothed_dft_buffers().last_set());
        //        m_line_renderer.fill_around_circle(m_sound_window->smoothed_dft_buffers().last_set());
        //        m_line_renderer.bind();
        //        glDrawElements(GL_LINE_STRIP, static_cast<GLsizei>(m_line_renderer.index_count()), GL_UNSIGNED_INT, nullptr);
        //        m_line_renderer.unbind();
    }

    void SmokeWidget::handle_beat(float beat_score) {
        if (beat_score < 0.6)
            return;

        beat_score = std::min(beat_score, 2.0f);
        for (auto& effect_detail : SmokeEffectManager::s_effects)
            if ((not effect_detail.is_on_cool_down) && math::bernoulli_trial(beat_score * effect_detail.probability)) {
                effect_detail.smoke_effect(*this, beat_score);
                effect_detail.start_cool_down();
            }

        for (auto& mover : m_auto_movers)
            mover.reset();
    }

    void SmokeWidget::try_adding_manipulator(Manipulator&& manipulator) {
        const auto same_lambda = [&](const auto& inner) {
            return std::visit([&](const auto& one) { return std::visit([&](const auto& two) { return one.manipulating_same(two); }, manipulator); }, inner);
        };

        // We check that the value we are trying to manipulate isn't already being manipulated
        if (std::find_if(m_manipulators.begin(), m_manipulators.end(), same_lambda) == m_manipulators.end())
            m_manipulators.emplace_back(manipulator);
    }

    void SmokeWidget::resizeGL(int w, int h) {
        m_smoke_renderer.resize(w, h);
        QOpenGLWidget::resizeGL(w, h);
    }

} // namespace app::disp