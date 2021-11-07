//
// Created by pc on 07-11-21.
//

#include "MainWidget.h"

#include "Math.h"

#include <QMouseEvent>
#include <cmath>

namespace app::disp {

    static const GLchar* g_vertex_source = R"glsl(
    #version 430 core
    in vec2 position;
    in vec3 color;
    out vec3 Color;
    uniform float offset = 1.0;

    vec3 f(float r, float g, float b){
        return 0.5 * vec3(r,g,b) + 0.5 *vec3(1.0f - g, 1.0f - b, 1.0f - r);
    }

    vec3 f2(float r, float g, float b){
        float c =  0.5 + 0.5 * sin(offset);
        return c * vec3(r, g, 1.0f - b) + (1.0f - c) * vec3(1.0f - g, b, 1.0f - r);
    }

    void main()
    {
//        gl_Position = vec4(position * (1.0f + 0.12 * abs(sin(offset))), 0.0, 1.0) ;
        gl_Position = vec4(position, 0.0, 1.0) ;
//        if ( mod(round(100 * gl_Position.x + 100 * gl_Position.y), 5) == 1){
//            Color = 0.21 * f2(color[0],color[0],color[0]);
//        } else {
            Color = f2(color[0],color[1],color[2]);
//        }
    }
)glsl";

    static const GLchar* g_fragment_source = R"glsl(
    #version 430 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";

    MainWidget::MainWidget(QWidget* parent) : QOpenGLWidget(parent), m_index_buf(QOpenGLBuffer::IndexBuffer) {
    }

    MainWidget::~MainWidget() {
        m_array_buf.destroy();
        m_index_buf.destroy();
    }

    void MainWidget::initializeGL() {
        initializeOpenGLFunctions();
        glClearColor(0, 0, 0, 1);
        init_shaders();
        init_buffers_and_vectors();

        m_timer.start(6, this);
    }

    void MainWidget::init_buffers_and_vectors() {
        m_array_buf.create();
        m_index_buf.create();

        m_vertices.resize(5 * m_horizontal_points * m_vertical_points);
        m_indices.resize(4 * (m_horizontal_points - 1) * (m_vertical_points - 1));

        for (size_t j = 0; j != m_vertical_points; ++j) {
            for (size_t i = 0; i != m_horizontal_points; ++i) {
                const size_t index    = 5 * (i + j * m_horizontal_points);
                m_vertices[index]     = i * 2.0f / static_cast<float>(m_horizontal_points - 1) - 1.0f;
                m_vertices[index + 1] = j * 2.0f / static_cast<float>(m_vertical_points - 1) - 1.0f;
            }
        }

        size_t index = 0;
        for (size_t i = 0; i != m_horizontal_points - 1; ++i) {
            for (size_t j = 0; j != m_vertical_points - 1; ++j) {
                size_t current_point = i + j * m_horizontal_points;
                m_indices[index]     = current_point;
                m_indices[index + 1] = current_point + m_horizontal_points;
                m_indices[index + 2] = current_point + 1 + m_horizontal_points;
                m_indices[index + 3] = current_point + 1;
                index += 4;
            }
        }
    }

    void MainWidget::init_shaders() {
        if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, g_vertex_source)) {
            close();
        }
        if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, g_fragment_source)) {
            close();
        }
        if (!m_program.link()) {
            close();
        }
        if (!m_program.bind()) {
            close();
        }
        m_offset_uniform = m_program.uniformLocation("offset");
    }

    void MainWidget::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program.setUniformValue(m_offset_uniform, m_offset);

        int position_location = m_program.attributeLocation("position");
        m_program.enableAttributeArray(position_location);
        m_program.setAttributeBuffer(position_location, GL_FLOAT, 0, 2, 5 * sizeof(float));

        int color_location = m_program.attributeLocation("color");
        m_program.enableAttributeArray(color_location);
        m_program.setAttributeBuffer(color_location, GL_FLOAT, 2 * sizeof(float), 3, 5 * sizeof(float));

        glDrawElements(GL_QUADS, m_array_buf.size() / 5, GL_UNSIGNED_INT, 0);
    }

    float project(float x) {
        x = math::clamp(x, -1.0f, 1.0f);
        if (x < 0.01f) {
            x = 0.5f * (1.0f - x) + 0.2f;
        }
                return math::clamp(std::sqrt(x), 0.0f, 1.0f);
//        return std::round(3.0f * std::sqrt(x)) / 3.0f;
    }

    void MainWidget::timerEvent(QTimerEvent* e) {
        m_offset += 0.16;
        //        m_fluid.add_bar();
        m_fluid.step(0.005);
        for (size_t j = 0; j != m_vertical_points; ++j) {
            for (size_t i = 0; i != m_horizontal_points; ++i) {
                const int index = 5 * (i + j * m_horizontal_points);
                if ((i + j) & 1) {
                    const auto r          = project(m_fluid.sample_density_at(i / static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 450.0f);
                    const auto g          = project(m_fluid.sample_u_at(i / static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 12.0f);
                    const auto b          = project(m_fluid.sample_v_at(i / static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 12.0f);
                    m_vertices[index + 2] = r;
                    m_vertices[index + 3] = r * g;
                    m_vertices[index + 4] = r * b;
                } else {
                    const auto r          = project(m_fluid.sample_density_at(i / static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 150.0f);
                    const auto g          = project(m_fluid.sample_u_at(i / static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 1.0f);
                    const auto b          = project(m_fluid.sample_v_at(i / static_cast<float>(m_horizontal_points), j / static_cast<float>(m_vertical_points)) / 1.0f);
                    m_vertices[index + 2] = r * b;
                    m_vertices[index + 3] = r;
                    m_vertices[index + 4] = r * g;
                }
            }
        }

        m_array_buf.bind();
        m_array_buf.allocate(m_vertices.data(), m_vertices.size() * sizeof(float));
        m_index_buf.bind();
        m_index_buf.allocate(m_indices.data(), m_indices.size() * sizeof(GLuint));
        m_fluid.clear_previous();
        update();
    }

    void MainWidget::mouseMoveEvent(QMouseEvent* e) {
        if (not(m_left_pressed || m_right_pressed)) {
            return;
        }
        const float width              = static_cast<float>(size().width());
        const float height             = static_cast<float>(size().height());
        const auto  new_mouse_position = QVector2D(e->localPos());
        QVector2D   direction          = new_mouse_position - m_mouse_last_position;
        direction[0] /= width;
        direction[1] /= height;

        if (m_left_pressed) {
            m_fluid.add_density(new_mouse_position.x() / width, 1.0f - new_mouse_position.y() / height);
        } else if (m_right_pressed) {
            m_fluid.add_density(new_mouse_position.x() / width, 1.0f - new_mouse_position.y() / height, -1.0f);
        }
        m_fluid.add_velocity(new_mouse_position.x() / width, 1.0f - new_mouse_position.y() / height, direction.x(), -direction.y());
        m_mouse_last_position = new_mouse_position;
    }

    void MainWidget::mousePressEvent(QMouseEvent* e) {
        switch (e->button()) {
            case Qt::LeftButton:
                m_left_pressed = true;
                break;
            case Qt::RightButton:
                m_right_pressed = true;
                break;
            default:
                break;
        }
        m_mouse_last_position = QVector2D(e->localPos());
    }

    void MainWidget::mouseReleaseEvent(QMouseEvent* e) {
        switch (e->button()) {
            case Qt::LeftButton:
                m_left_pressed = false;
                break;
            case Qt::RightButton:
                m_right_pressed = false;
                break;
            default:
                break;
        }
    }

} // namespace app::disp