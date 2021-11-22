//
// Created by pc on 18-11-21.
//

#ifndef H_APP_DISP_LINESHADER_H
#define H_APP_DISP_LINESHADER_H

#include <QOpenGLShaderProgram>
namespace app::disp {

    class LineShader {

      public:
        bool init();
        void update(float dt);
        void set_locations();
        void zoom();
        void set_uniforms();
        void unbind();
        void bind();

      private:
        float                m_zoom = 0.0f;
        QOpenGLShaderProgram m_program;
    };

} // namespace app::disp

#endif // H_APP_DISP_LINESHADER_H
