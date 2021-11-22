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

    void RendererBase::init() {
        m_quad_array_buffer.create();
        m_quad_index_buffer.create();
    }

    void RendererBase::bind() {
        m_quad_array_buffer.bind();
        m_quad_array_buffer.allocate(m_quad_vertices.data(), static_cast<int>(m_quad_vertices.size() * sizeof(float)));
        m_quad_index_buffer.bind();
        m_quad_index_buffer.allocate(m_quad_indices.data(), static_cast<int>(m_quad_indices.size() * sizeof(GLuint)));
    }

    void RendererBase::unbind() {
        m_quad_array_buffer.release();
        m_quad_index_buffer.release();
    }

    size_t RendererBase::index_count() const {
        return m_quad_indices.size();
    }

} // namespace app::disp