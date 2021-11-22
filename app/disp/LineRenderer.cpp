
#include "LineRenderer.h"

#include "../Math.h"
#include "../tools/ThreadPool.h"
#include "../tools/ThreadSettings.h"
#include "LineShader.h"

#include <cmath>

namespace app::disp {

    LineRenderer::LineRenderer() {
        m_shader = std::unique_ptr<ShaderBase>(new LineShader{});
    }

    void LineRenderer::fill_around_circle(const std::vector<size_t>& buffer) {
        m_quad_vertices.clear();
        m_quad_indices.clear();
        const size_t n   = buffer.size();
        const size_t max = *std::max_element(buffer.begin(), buffer.end());
        for (size_t i = 0; i != n; ++i) {
            m_quad_vertices.push_back(0.5 * buffer[i] * std::sin((2 * M_PI * i) / n) / max);
            m_quad_vertices.push_back(0.5 * buffer[i] * std::cos((2 * M_PI * i) / n) / max);
            m_quad_vertices.push_back(1);
            m_quad_vertices.push_back(1);
            m_quad_vertices.push_back(0);
            m_quad_indices.push_back(i);
        }
        m_quad_indices.push_back(0);
    }

    void LineRenderer::fill_around_ellipse(const std::vector<size_t>& buffer, float a, float b, float theta, float dx = 0.0f, float dy = 0.0f) {
        m_quad_vertices.clear();
        m_quad_indices.clear();
        size_t       n   = buffer.size();
        const size_t max = *std::max_element(buffer.begin(), buffer.end());
        for (size_t i = 0; i != n; ++i) {
            auto [x, y] = app::math::rotate(0.5 * buffer[i] * a * std::sin((2 * M_PI * i) / n) / max, 0.5 * buffer[i] * b * std::cos((2 * M_PI * i) / n) / max, -theta);
            m_quad_vertices.push_back(x + dx);
            m_quad_vertices.push_back(y + dy);
            m_quad_vertices.push_back(1);
            m_quad_vertices.push_back(1);
            m_quad_vertices.push_back(0);
            m_quad_indices.push_back(i);
        }
        m_quad_indices.push_back(0);
    }

} // namespace app::disp