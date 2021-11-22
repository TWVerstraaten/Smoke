//
// Created by pc on 18-11-21.
//

#ifndef H_APP_DISP_SHADERS_PLAINFRAGMENTSHADER_H
#define H_APP_DISP_SHADERS_PLAINFRAGMENTSHADER_H

namespace app::disp {

    static const GLchar* g_plain_fragment_shader = R"glsl(
    #version 430 core
    in vec3 Color;

    out vec4 outColor;

    void main()
    {
        outColor = vec4(Color, 1.0f);
    }
)glsl";

}

#endif // H_APP_DISP_SHADERS_PLAINFRAGMENTSHADER_H
