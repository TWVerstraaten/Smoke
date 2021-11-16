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
        void zoom();
        void set_circle();
        void set_uniform_screen_size(int width, int height);

      private:
        float                m_offset       = 0.0f;
        float                m_zoom         = 0.0f;
        float                m_circle_timer = 0.0f;
        QOpenGLShaderProgram m_program;
    };

} // namespace app::disp

#endif // H_APP_DISP_SHADER_H
