//
// Created by pc on 22-11-21.
//

#include "RendererBase.h"

namespace app::disp {

    RendererBase::RendererBase() : m_quad_index_buffer(QOpenGLBuffer::IndexBuffer) {
    }

    RendererBase::~RendererBase() {
        m_quad_index_buffer.destroy();
        m_quad_index_buffer.destroy();
    }

    bool RendererBase::init() {
        m_is_initialized = m_quad_array_buffer.create() && m_quad_index_buffer.create() && m_shader->init();
        return m_is_initialized;
    }

    void RendererBase::bind() {
        assert(m_is_initialized);
        m_quad_array_buffer.bind();
        m_quad_array_buffer.allocate(m_quad_vertices.data(), static_cast<int>(m_quad_vertices.size() * sizeof(float)));
        m_quad_index_buffer.bind();
        m_quad_index_buffer.allocate(m_quad_indices.data(), static_cast<int>(m_quad_indices.size() * sizeof(GLuint)));

        m_shader->bind();
        m_shader->set_uniforms();
        m_shader->set_locations();
    }

    void RendererBase::unbind() {
        m_quad_array_buffer.release();
        m_quad_index_buffer.release();
        m_shader->unbind();
    }

    size_t RendererBase::index_count() const {
        return m_quad_indices.size();
    }

} // namespace app::disp