//
// Created by pc on 18-11-21.
//

#ifndef H_APP_DISP_LINESHADER_H
#define H_APP_DISP_LINESHADER_H

#include "ShaderBase.h"

#include <QOpenGLShaderProgram>

namespace app::disp {

    class LineShader : public ShaderBase {

      public:
        bool init() override;
        void set_uniforms() override;
    };

} // namespace app::disp

#endif // H_APP_DISP_LINESHADER_H
