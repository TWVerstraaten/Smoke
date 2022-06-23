//
// Created by pc on 22-11-21.
//

#ifndef H_APP_DISP_RENDERERBASE_H
#define H_APP_DISP_RENDERERBASE_H

#include "ShaderBase.h"

#include <QOpenGLBuffer>
#include <memory>
#include <vector>

namespace app::disp {

    class RendererBase {

      public:
        void                 unbind();
        void                 bind();
        [[nodiscard]] bool   init();
        [[nodiscard]] size_t index_count() const;

      protected:
        RendererBase();
        ~RendererBase();

        std::vector<GLuint>         m_quad_indices{};
        std::vector<float>          m_quad_vertices{};
        std::unique_ptr<ShaderBase> m_shader{};

      private:
        bool          m_is_initialized = false;
        QOpenGLBuffer m_quad_index_buffer;
        QOpenGLBuffer m_quad_array_buffer{};
    };

} // namespace app::disp

#endif // H_APP_DISP_RENDERERBASE_H
