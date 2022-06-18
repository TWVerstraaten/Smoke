//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_SMOKESHADER_H
#define H_APP_DISP_SMOKESHADER_H

#include "ShaderBase.h"

namespace app::fluid {
    class Fluid;
}

namespace app::disp {

    class SmokeShader : public ShaderBase {

      public:
        void set_extreme_values(const app::fluid::Fluid& fluid);

        bool init() override;
        void set_uniforms() override;

      private:
        float m_max_density;
        float m_min_density;
        float m_max_v;
        float m_min_v;
        float m_max_u;
        float m_min_u;
    };

} // namespace app::disp

#endif // H_APP_DISP_SMOKESHADER_H
