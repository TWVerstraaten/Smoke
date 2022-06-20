//
// Created by pc on 07-11-21.
//

#include "SmokeWidget.h"

#include "../../fluid/Fluid.h"
#include "../State.h"
#include "../misc/Overloaded.h"
#include "../prf/Profile.h"
#include "DispSettings.h"

#include <QMouseEvent>

namespace app::disp {

    SmokeWidget::SmokeWidget(QWidget* parent) : QOpenGLWidget(parent), m_fluid(new fluid::Fluid) {
        m_auto_movers.resize(1);

        m_shots.emplace_back(Shot{[this](float beat_score) {
                                      zoom(beat_score / 30);
                                      set_random_shape();
                                  },
                                  0.2});

        m_shots.emplace_back(Shot{[this](float beat_score) { try_adding_manipulator(ManipulatorBase<float>(10, 0.0f, 0.9f, &g_clamp_coefficient)); }, 0.8});
        m_shots.emplace_back(Shot{[this](float beat_score) { g_invert_colors = !g_invert_colors; }, 0.1});
        m_shots.emplace_back(Shot{[this](float beat_score) {
                                      m_auto_movers.resize(0);
                                      m_auto_movers.resize(rand() % 2);
                                  },
                                  0.4});

        m_shots.emplace_back(Shot{[this](float beat_score) { try_adding_manipulator(ManipulatorBase<size_t>(0.4f * beat_score, 6, 2, &g_clamp_count)); }, 0.6});
        m_shots.emplace_back(
            Shot{[this](float beat_score) { try_adding_manipulator(ManipulatorBase<float>(0.4f * beat_score, 0.4f - (beat_score / 10), 0.7, &g_power_scale)); }, 0.6});
        m_shots.emplace_back(
            Shot{[this](float beat_score) { try_adding_manipulator(ManipulatorBase<float>(0.4f * beat_score, 0.9f, g_clamp_coefficient, &g_clamp_coefficient)); }, 0.6});
        m_shots.emplace_back(Shot{[this](float beat_score) { try_adding_manipulator(ManipulatorBase<size_t>(0.2f * beat_score, 12, 6, &g_pixel_size)); }, 0.04});
    }

    SmokeWidget::~SmokeWidget() = default;

    void SmokeWidget::initializeGL() {
        initializeOpenGLFunctions();
        QOpenGLFunctions::glEnable(GL_POINT_SIZE);
        glClearColor(0, 0, 0, 1);
        if ((not m_smoke_renderer.init()) || (not m_line_renderer.init()))
            close();
    }

    void SmokeWidget::paintGL() {
        PROFILE_FUNCTION();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw_smoke();
        draw_lines();
        //        PRINT_PROFILE();
        START_PROFILING();
    }

    void SmokeWidget::step(float elapsed) {
        PROFILE_FUNCTION();
        if (not State::get().is_paused()) {
            ShaderBase::update_statics(elapsed);

            for (auto& auto_mover : m_auto_movers) {
                auto_mover.update(elapsed);
                m_fluid->add_density(auto_mover.position(), 0.5);
                m_fluid->add_velocity(auto_mover.position(), 0.04 * auto_mover.velocity());
                //                m_fluid->add_density(auto_mover.target_position());
            }

            m_manipulators.erase(std::remove_if(m_manipulators.begin(),
                                                m_manipulators.end(),
                                                [](const auto& v) { return std::visit(misc::Overloaded{[](const auto& obj) { return obj.is_done(); }}, v); }),
                                 m_manipulators.end());

            for (auto& manipulator : m_manipulators) {
                std::visit(misc::Overloaded{[elapsed](auto& a) { a.update(elapsed); }}, manipulator);
            }

            m_fluid->step();
        }
        update();
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

    void SmokeWidget::set_random_shape() {
        //        m_fluid->set_circle();
        m_fluid->set_polygon(3 + (rand() % 12), rand() / static_cast<float>(RAND_MAX));
    }

    void SmokeWidget::zoom(float zoom_depth) {
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
    }

    void SmokeWidget::handle_beat(float beat_score) {
        try_adding_manipulator(ManipulatorBase<size_t>(0.4f, 6, 2, &g_clamp_count));
        if (beat_score < 0.01)
            return;

        beat_score = std::min(beat_score, 2.0f);

        for (const auto& shot : m_shots) {
            if (rand() < static_cast<float>(RAND_MAX) * shot.m_probability)
                shot.m_func(beat_score);
        }

        for (auto& mover : m_auto_movers) {
            float p = beat_score / 10;
            if (rand() < p * static_cast<float>(RAND_MAX))
                mover.reset();
        }
    }

    void SmokeWidget::try_adding_manipulator(Manipulator&& manipulator) {
        static auto same_lambda = [](const auto& man, const auto& inner) { return std::visit(misc::Overloaded{[&](const auto& in) { return man.manipulating_same(in); }}, inner); };
        static auto same_exists_in_vector = [&](const auto& man) {
            return std::find_if(m_manipulators.begin(), m_manipulators.end(), [&](const auto& inner) { return same_lambda(man, inner); }) != m_manipulators.end();
        };

        if (not std::visit(same_exists_in_vector, manipulator))
            m_manipulators.emplace_back(manipulator);
    }

} // namespace app::disp