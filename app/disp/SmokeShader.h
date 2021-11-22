//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_SMOKESHADER_H
#define H_APP_DISP_SMOKESHADER_H

#include "ShaderBase.h"

namespace app::disp {

    class SmokeShader : public ShaderBase {

      public:
        bool init() override;
        void set_uniforms() override;
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKESHADER_H
