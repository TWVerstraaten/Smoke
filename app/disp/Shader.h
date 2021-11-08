//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_SHADER_H
#define H_APP_DISP_SHADER_H

#include <QOpenGLShaderProgram>

namespace app::disp {

    class Shader {

      public:
        bool init();
        void update(float dt);
        void set_locations();

      private:
        float                m_offset         = 0.0f;
        int                  m_offset_uniform = -1;
        QOpenGLShaderProgram m_program;
    };

} // namespace app::disp

#endif // H_APP_DISP_SHADER_H
