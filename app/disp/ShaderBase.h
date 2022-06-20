//
// Created by pc on 22-11-21.
//

#ifndef H_APP_DISP_SHADERBASE_H
#define H_APP_DISP_SHADERBASE_H

#include <QOpenGLShaderProgram>

namespace app::disp {

    class ShaderBase {

      public:
        virtual ~ShaderBase() = default;

        void bind();
        void unbind();
        void set_locations();

        virtual bool                  init() = 0;
        virtual void                  set_uniforms();
        [[maybe_unused]] virtual void update(float dt);

        static void zoom(float zoom_depth);
        static void update_statics(float dt);

      protected:
        [[nodiscard]] bool init(const char* vertex_shader, const char* fragment_shader);

        QOpenGLShaderProgram m_program;
        inline static float  s_zoom = 0.0f;
        inline static float  s_time = 0.0f;
    };

} // namespace app::disp

#endif // H_APP_DISP_SHADERBASE_H
