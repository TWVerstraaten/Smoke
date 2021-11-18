//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_SMOKESHADER_H
#define H_APP_DISP_SMOKESHADER_H

#include <QOpenGLShaderProgram>

namespace app::disp {

    class SmokeShader {

      public:
        bool init();
        void update(float dt);
        void set_locations();
        void zoom();
        void set_circle();
        void set_uniforms();
        void unbind();
        void bind();

      private:
        float                m_offset       = 0.0f;
        float                m_zoom         = 0.0f;
        float                m_circle_timer = 0.0f;
        float                m_time         = 0.0f;
        QOpenGLShaderProgram m_program;
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKESHADER_H
