//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_SMOKERENDERER_H
#define H_APP_DISP_SMOKERENDERER_H

#include <QOpenGLBuffer>
#include <vector>

namespace app::fl {
    class Fluid;
}

namespace app::disp {

    class SmokeRenderer {

      public:
        SmokeRenderer();
        ~SmokeRenderer();

        void                 init();
        void                 bind_quads();
        void                 unbind();
        void                 fill_quads(const app::fl::Fluid& fluid);
        void                 set_sample_points(size_t width, size_t height);
        [[nodiscard]] size_t index_count() const;

      private:
        void init_buffers_and_vectors();
        void fill_quads_normal(const app::fl::Fluid& fluid);
        void fill_quads_pixel(const app::fl::Fluid& fluid);

        QOpenGLBuffer       m_quad_array_buffer{};
        QOpenGLBuffer       m_quad_index_buffer;
        std::vector<float>  m_quad_vertices{};
        std::vector<GLuint> m_quad_indices{};
        size_t              m_vertical_sample_points{};
        size_t              m_horizontal_sample_points{};
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKERENDERER_H
