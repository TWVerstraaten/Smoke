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

    out vec3 out_color;

    uniform bool  invert_colors = false;
    uniform int   color_mode = 0;
    uniform int   clamp_count = 3;
    uniform float time = 1;
    uniform float zoom = 1.0;
    uniform float clamp_coefficient = 0.0f;
    uniform float power_scale = 0.5f;
    uniform float color_wash = 0.0f;
    uniform float max_density;
    uniform float min_density;
    uniform float max_v;
    uniform float min_v;
    uniform float max_u;
    uniform float min_u;

    float clamp_to_levels(float c) {
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

    bool on(vec2 coords) {
        return length(vec2(mod(coords.x, 0.1), mod(coords.y, 0.1))) < (0.1 + 0.1 * sin(2 * time));
    }

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0 / zoom);

        if (max_density - min_density <= 0.01)
            out_color = vec3(0,0,0);
        else {
            if (color_mode == 0) // COLOR_MODE::GRAY
                out_color = 0.0025 * vec3(color.r, color.r, color.r);
            else {
                float hue        = atan(color.g, color.b) * 360.0 / 3.14;
                float saturation = 0.8 * sqrt(color.g * color.g + color.b * color.b);
                float value      = 0.002 * color.r + 0.4 * saturation;
                if (color_mode == 1) {                // COLOR_MODE::HSL
                    out_color = hslToRgb(hue, saturation, value);
                    out_color = out_color.rrb;
                    out_color = out_color.bgr;
                }
                else if (color_mode == 2){           // COLOR_MODE::HSV
                    out_color = 0.0025 * vec3(color.r, color.r, color.r);
                }
                else if (color_mode == 3) {
          // COLOR_MODE::WATER
                    out_color = 0.8 * vec3(0.24 * abs(value) - 0.12 * saturation, 0.28 * abs(value) + abs(0.1 * saturation), 0.2 * abs(value) + abs(0.3 * saturation));
                }
                else if (color_mode == 4) {          // COLOR_MODE::TEST_1
                    out_color = 0.5 * vec3( value - 0.30 * color.g + 0.16 * color.b, value -  0.31 * color.g + 0.11 * color.b, value - 0.32 * color.g + 0.14 * color.b);
                }
                else if (color_mode == 5)           // COLOR_MODE::TEST_2
                    out_color = vec3(0.3,0.3,0.3) - vec3(1 - 0.002 * color.r + 0.2 * color.g, 1 - 0.3 * color.g - 0.1 * color.b, 1 - 0.7 * color.b - 0.0001 * color.r);
            }

            out_color = vec3(pow(out_color.r, power_scale), pow(out_color.g, power_scale), pow(out_color.b, power_scale));
            out_color = vec3(abs_clamp(out_color.r), abs_clamp(out_color.g), abs_clamp(out_color.b));

            if (invert_colors)
                out_color = vec3(1.0f - out_color.r, 1.0f - out_color.g, 1.0f - out_color.b);

            out_color = (1.0f - clamp_coefficient) * out_color + clamp_coefficient * vec3(clamp_to_levels(out_color.r), clamp_to_levels(out_color.g), clamp_to_levels(out_color.b));
            out_color = (1 - color_wash) * out_color + color_wash * vec3(1, 1, 1);

            out_color *= 0.94;
        }
    }
)glsl";
}

#endif // H_APP_DISP_SHADERS_SMOKEVERTEXSHADER_H
