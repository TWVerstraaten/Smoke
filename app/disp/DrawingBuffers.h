//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_DRAWINGBUFFERS_H
#define H_APP_DISP_DRAWINGBUFFERS_H

#include <QOpenGLBuffer>

namespace app::fl {
    class Fluid;
}

namespace app::disp {

    class DrawingBuffers {
      public:
        DrawingBuffers();
        ~DrawingBuffers();

        void init();
        void fill_and_bind(const app::fl::Fluid& m_fluid);

        [[nodiscard]] size_t quad_count() const;

      private:
        void init_buffers_and_vectors();

        QOpenGLBuffer       m_array_buffer;
        QOpenGLBuffer       m_index_buffer;
        std::vector<float>  m_vertices;
        std::vector<GLuint> m_indices;
        const size_t        m_vertical_points   = 500;
        const size_t        m_horizontal_points = 500;
    };

} // namespace app::disp

#endif // H_APP_DISP_DRAWINGBUFFERS_H
