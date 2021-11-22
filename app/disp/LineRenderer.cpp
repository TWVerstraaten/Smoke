
#include "LineRenderer.h"

#include "../Math.h"
#include "../fl/Fluid.h"
#include "../tools/Profile.h"
#include "../tools/ThreadPool.h"
#include "../tools/ThreadSettings.h"
#include "DisplaySettings.h"
#include "ShaderSettings.h"

#include <cmath>

namespace app::disp {

    LineRenderer::LineRenderer() : m_line_index_buffer(QOpenGLBuffer::IndexBuffer) {
    }

    LineRenderer::~LineRenderer() {
        m_line_array_buffer.destroy();
        m_line_index_buffer.destroy();
    }

    void LineRenderer::init() {
        init_buffers_and_vectors();
    }

    void LineRenderer::init_buffers_and_vectors() {
        m_line_array_buffer.create();
        m_line_index_buffer.create();
    }

    size_t LineRenderer::index_count() const {
        return m_line_indices.size();
    }

    void LineRenderer::fill_around_circle(const std::vector<size_t>& buffer) {
        m_line_vertices.clear();
        m_line_indices.clear();
        size_t       n   = buffer.size();
        const size_t max = *std::max_element(buffer.begin(), buffer.end());
        for (size_t i = 0; i != n; ++i) {
            m_line_vertices.push_back(0.5 * buffer[i] * std::sin((2 * M_PI * i) / n) / max);
            m_line_vertices.push_back(0.5 * buffer[i] * std::cos((2 * M_PI * i) / n) / max);
            m_line_vertices.push_back(1);
            m_line_vertices.push_back(1);
            m_line_vertices.push_back(0);
            m_line_indices.push_back(i);
        }
        m_line_indices.push_back(0);
    }

    void LineRenderer::fill_around_ellipse(const std::vector<size_t>& buffer, float a, float b, float theta, float dx = 0.0f, float dy = 0.0f) {
        m_line_vertices.clear();
        m_line_indices.clear();
        size_t       n   = buffer.size();
        const size_t max = *std::max_element(buffer.begin(), buffer.end());
        for (size_t i = 0; i != n; ++i) {
            auto [x, y] = app::math::rotate(0.5 * buffer[i] * a * std::sin((2 * M_PI * i) / n) / max, 0.5 * buffer[i] * b * std::cos((2 * M_PI * i) / n) / max, -theta);
            m_line_vertices.push_back(x + dx);
            m_line_vertices.push_back(y + dy);
            m_line_vertices.push_back(1);
            m_line_vertices.push_back(1);
            m_line_vertices.push_back(0);
            m_line_indices.push_back(i);
        }
        m_line_indices.push_back(0);
    }

    void LineRenderer::bind_lines() {
        m_line_array_buffer.bind();
        m_line_array_buffer.allocate(m_line_vertices.data(), m_line_vertices.size() * sizeof(float));
        m_line_index_buffer.bind();
        m_line_index_buffer.allocate(m_line_indices.data(), m_line_indices.size() * sizeof(GLuint));
    }

    void LineRenderer::unbind() {
        m_line_array_buffer.release();
        m_line_index_buffer.release();
    }

} // namespace app::disp