//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_SMOKERENDERER_H
#define H_APP_DISP_SMOKERENDERER_H

#include <QOpenGLBuffer>

namespace app::fl {
    class Fluid;
}

namespace app ::tools {
    class Buffer;
}

namespace app::disp {

    class SmokeRenderer {
      public:
        SmokeRenderer();
        ~SmokeRenderer();

        void                 init();
        void                 bind_quads();
        void                 unbind();
        void                 fill_quads(const app::fl::Fluid& m_fluid);
        [[nodiscard]] size_t quad_count() const;

      private:
        void init_buffers_and_vectors();

        QOpenGLBuffer       m_quad_array_buffer;
        QOpenGLBuffer       m_quad_index_buffer;
        std::vector<float>  m_quad_vertices;
        std::vector<GLuint> m_quad_indices;
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKERENDERER_H
