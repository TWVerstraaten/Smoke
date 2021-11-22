//
// Created by pc on 22-11-21.
//

#ifndef H_APP_DISP_RENDERERBASE_H
#define H_APP_DISP_RENDERERBASE_H

#include <QOpenGLBuffer>
#include <vector>

namespace app::disp {

    class RendererBase {

      public:
        RendererBase();
        ~RendererBase();

        void                 init();
        void                 bind();
        void                 unbind();
        [[nodiscard]] size_t index_count() const;

      protected:
        std::vector<GLuint> m_quad_indices{};
        std::vector<float>  m_quad_vertices{};

      private:
        QOpenGLBuffer m_quad_index_buffer;
        QOpenGLBuffer m_quad_array_buffer{};
    };

} // namespace app::disp

#endif // H_APP_DISP_RENDERERBASE_H
