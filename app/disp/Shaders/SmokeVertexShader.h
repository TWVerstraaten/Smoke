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

    uniform float zoom = 1.0;
    uniform float clamp_coefficient = 0.0f;
    uniform float power_scale = 0.5f;
    uniform int   color_mode = 0;
    uniform int   clamp_count = 3;
    uniform bool  invert_colors = false;

    vec3 f(float r, float g, float b){
        return 0.5 * vec3(r,g,b) + 0.5 *vec3(1.0f - g, 1.0f - b, 1.0f - r);
    }

    vec3 f2(float r, float g, float b){
        float c =  0.5;
        return c * vec3(r, g, 1.0f - b) + (1.0f - c) * vec3(1.0f - g, b, 1.0f - r);
    }

    float project(float c){
        return  float( int(clamp_count * c)) / (clamp_count - 1);
    }

    float abs_clamp(float c){
        return clamp(abs(c), 0.0f, 1.0f);
    }

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0 / zoom);

        Color = vec3(abs_clamp(color.r), abs_clamp(color.g), abs_clamp(color.b));
        Color = vec3(pow(Color.r, power_scale), pow(Color.g, power_scale), pow(Color.b, power_scale));
        if (invert_colors){
            Color = vec3(1.0f - Color.r, 1.0f - Color.g, 1.0f - Color.b);
        }
        Color = (1.0f - clamp_coefficient) * Color + clamp_coefficient * vec3(project(Color.r), project(Color.g), project(Color.b));
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
