//
// Created by pc on 17-11-21.
//

#ifndef H_APP_DISP_SHADERS_SMOKEFRAGMENTSHADER_H
#define H_APP_DISP_SHADERS_SMOKEFRAGMENTSHADER_H

namespace app::disp {

    static const GLchar* g_smoke_fragment_shader = R"glsl(
    #version 430 core
    in vec3 out_color;

    out vec4 outColor;

    uniform float time = 1;

    void main()
    {
        outColor = vec4(out_color, 1.0f);
    }
)glsl";

}

#endif // H_APP_DISP_SHADERS_SMOKEFRAGMENTSHADER_H
