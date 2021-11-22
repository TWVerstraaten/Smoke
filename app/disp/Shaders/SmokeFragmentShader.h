//
// Created by pc on 17-11-21.
//

#ifndef H_APP_DISP_SHADERS_SMOKEFRAGMENTSHADER_H
#define H_APP_DISP_SHADERS_SMOKEFRAGMENTSHADER_H

namespace app::disp {

    static const GLchar* g_smoke_fragment_shader = R"glsl(
    #version 430 core
    in vec3 Color;
    in vec2 normCoord;

    out vec4 outColor;

    uniform float time = 403.12;

    float r(vec2 co){
        return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
    }

    float step = 8.0;
    float density = 0.03;

    bool on(vec2 co, float ent){
        float x = floor(co.x / step);
        float y = floor(co.y / step);
        float offset = r(vec2(x,y));
        return mod(ent + offset, 1.0f) < density;
    }

    bool red_circ(vec2 co, float ent){
        if (on(co, ent)){
            float x = step * floor(co.x / float(step)) + step / 2.0;
            float y = step * floor(co.y / float(step)) + step / 2.0;
            float dx = x - co.x;
            float dy = y - co.y;
            if (4.0 * (dx*dx + dy*dy) < float(step * step)){
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    bool red_square(vec2 co, float ent){
        if (on(co, ent)){
            return true;
        } else {
            return false;
        }
    }

    void main()
    {
        outColor = vec4(Color, 1.0f);
//        float c = 0.5 + clamp(0.9f * sin((normCoord.x + normCoord.y) + 2.2 * time), -0.5f, 0.5f);
//        outColor = c * vec4(Color, 1.0) + (1-c) * vec4(1.0f - Color.gbr, 1.0);
//
//        if (c > 0.01 && c < 0.99){
//            if (red_square(gl_FragCoord.xy, time / 10.0)){
//                outColor = vec4(1.0f - outColor.rgb, 1.0);
//            } else {
//            }
//        }
    }
)glsl";

}

#endif // H_APP_DISP_SHADERS_SMOKEFRAGMENTSHADER_H
