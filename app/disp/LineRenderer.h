//
// Created by pc on 18-11-21.
//

#ifndef H_APP_DISP_LINERENDERER_H
#define H_APP_DISP_LINERENDERER_H

#include <QOpenGLBuffer>
#include <vector>

namespace app::disp {

    class LineRenderer {
      public:
        LineRenderer();
        ~LineRenderer();

        void                 init();
        void                 bind_lines();
        void                 unbind();
        void                 fill_around_circle(const std::vector<size_t>& buffer);
        void                 fill_around_ellipse(const std::vector<size_t>& buffer, float a, float b, float theta, float dx, float dy);
        [[nodiscard]] size_t index_count() const;

      private:
        void init_buffers_and_vectors();

        QOpenGLBuffer       m_line_array_buffer;
        QOpenGLBuffer       m_line_index_buffer;
        std::vector<float>  m_line_vertices;
        std::vector<GLuint> m_line_indices;
    };

} // namespace app::disp

#endif // H_APP_DISP_LINERENDERER_H
