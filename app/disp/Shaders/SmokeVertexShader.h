//
// Created by pc on 17-11-21.
//

#ifndef H_APP_DISP_SHADERS_SMOKEVERTEXSHADER_H
#define H_APP_DISP_SHADERS_SMOKEVERTEXSHADER_H

namespace app::disp {

    static const GLchar* g_smoke_vertex_shader = R"glsl(
    #version 430 core
    in vec2 position;
    in vec3 color;

    out vec3 Color;
    out vec2 normCoord;

    uniform float offset = 0.0;
    uniform float zoom = 1.0;
    uniform int   color_mode = 0;
    uniform bool  invert_colors = false;

    vec3 f(float r, float g, float b){
        return 0.5 * vec3(r,g,b) + 0.5 *vec3(1.0f - g, 1.0f - b, 1.0f - r);
    }

    vec3 f2(float r, float g, float b){
        float c =  0.5 + 0.5 * sin(offset);
        return c * vec3(r, g, 1.0f - b) + (1.0f - c) * vec3(1.0f - g, b, 1.0f - r);
    }

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0 / zoom);
        if (invert_colors){
            Color = vec3(1.0f - color.r, 1.0f - color.g, 1.0f - color.b);
        } else {
            Color = color;
        }
        if (color_mode == 0){
            Color = Color.rrr;
        } else if (color_mode == 1){
            Color = Color;
        } else if (color_mode == 2){
            Color = f2(Color.r, Color.g, Color.b);
        }
        normCoord = position.xy;
    }
)glsl";

}

#endif // H_APP_DISP_SHADERS_SMOKEVERTEXSHADER_H
