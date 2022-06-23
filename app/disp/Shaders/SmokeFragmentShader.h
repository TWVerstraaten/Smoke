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
    uniform int width;
    uniform int height;

    void main()
    {
        outColor = vec4(out_color, 1.0f);

//        if ((gl_FragCoord.x - width/4) * (gl_FragCoord.y - height/2) > 0)
//            outColor = vec4(outColor.r, 0.0, outColor.b, 1.0f);

        if (length((gl_FragCoord.xy - vec2(width/2,height/2))) >  0.5 * min(width, height) )
            outColor = vec4(1.4 * outColor.r, 1.1 * outColor.g, 1.3 * outColor.b, 1.0f);

    }
)glsl";

}

#endif // H_APP_DISP_SHADERS_SMOKEFRAGMENTSHADER_H
