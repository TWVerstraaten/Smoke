//
// Created by pc on 17-11-21.
//

#ifndef H_APP_DISP_SHADERS_SMOKEFRAGMENTSHADER_H
#define H_APP_DISP_SHADERS_SMOKEFRAGMENTSHADER_H

namespace app::disp {

    static const GLchar* g_smoke_fragment_shader = R"glsl(
    #version 430 core
    in vec3 Color;

    out vec4 outColor;

    uniform float time = 1;

//    bool on(vec2 coords){
//        if (length(vec2(mod(coords.x, 20), mod(coords.y, 20))) < (14 + 5 * sin(10 * time))){
//            return true;
//        } else {
//            return false;
//        }
//    }

    void main()
    {
        outColor = vec4(Color, 1.0f);

//        if (on(gl_FragCoord.xy)) {
//            outColor = vec4(1.0 - Color.r, 1.0 - Color.g, 1.0 - Color.b, 1.0f);
//        } else {
//            outColor = vec4(Color, 1.0f);
//        }

    }
)glsl";

}

#endif // H_APP_DISP_SHADERS_SMOKEFRAGMENTSHADER_H
