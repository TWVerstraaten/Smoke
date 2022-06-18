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

    uniform float time = 1;
    uniform float zoom = 1.0;
    uniform float clamp_coefficient = 0.0f;
    uniform float power_scale = 0.5f;
    uniform int   color_mode = 0;
    uniform int   clamp_count = 3;
    uniform bool  invert_colors = false;
    uniform float max_density;
    uniform float min_density;
    uniform float max_v;
    uniform float min_v;
    uniform float max_u;
    uniform float min_u;

    float project(float c){
        return  float( int(clamp_count * c)) / (clamp_count - 1);
    }

    float abs_clamp(float c){
        return clamp(abs(c), 0.0f, 1.0f);
    }

    float fHsl(int n, float h, float s, float l){
        float k = mod(n + h / 30, 12);
        float a = s * min(l, 1 - l);
        return l - a * max(-1, min(k - 3, min(9 - k, 1)));
    }

    vec3 hslToRgb(float h, float s, float l){
        return vec3(fHsl(0, h, s, l), fHsl(8, h, s, l), fHsl(4, h, s, l));
    }

    float fHsv(int n, float h, float s, float v){
        float k = mod(n + h / 60, 6);
        return v - v * s * max(0, min(k, min(4 - k, 1)));
    }

    vec3 hsvToRgb(float h, float s, float v){
        return vec3(fHsv(5, h, s, v), fHsv(3, h, s, v), fHsv(1, h, s, v));
    }

    bool on(vec2 coords){
        if (length(vec2(mod(coords.x, 0.1), mod(coords.y, 0.1))) < (0.1 + 0.1 * sin(2 * time))){
            return true;
        } else {
            return false;
        }
    }

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0 / zoom);

        if (max_density - min_density <= 0.01) {
            Color = vec3(0,0,0);
        } else {
            if (color_mode == 0){
                Color = 0.0025 * vec3(color.r, color.r, color.r);
            } else {
                float hue = atan(color.g, color.b) * 360.0 / 3.14;
                float saturation = 4 * sqrt(color.g * color.g + color.b * color.b);
                float value = 0.002 * color.r + 0.4 * saturation;
                bool b = on(gl_Position.xy);
//                if (b) {
//                    hue += 150;
//                    value *= 1.5;
//                }
                hue += time;
                if (color_mode == 1) {
                    Color = hslToRgb(hue, saturation, value);
                } else if (color_mode == 2){
                    Color = hsvToRgb(hue, saturation, value);
                } else if (color_mode == 3){
                    Color = vec3(0.24 * abs(value) - 0.19 * saturation, 0.28 * abs(value) + abs(0.1 * saturation), 0.2 * abs(value) + abs(0.3 * saturation));
                }
            }

            Color = vec3(pow(Color.r, power_scale), pow(Color.g, power_scale), pow(Color.b, power_scale));
            Color = vec3(abs_clamp(Color.r), abs_clamp(Color.g), abs_clamp(Color.b));

            if (invert_colors){
                Color = vec3(1.0f - Color.r, 1.0f - Color.g, 1.0f - Color.b);
            }
            Color = (1.0f - clamp_coefficient) * Color + clamp_coefficient * vec3(project(Color.r), project(Color.g), project(Color.b));
//            if (on(gl_Position.xy)) {
//                Color = vec3(1.0 - Color.r, 1.0 - Color.g, 1.0 - Color.b);
//            }
        }

    }
)glsl";

}

#endif // H_APP_DISP_SHADERS_SMOKEVERTEXSHADER_H
