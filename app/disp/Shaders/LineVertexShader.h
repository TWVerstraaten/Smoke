//
// Created by pc on 18-11-21.
//

#ifndef H_APP_DISP_SHADERS_LINEVERTEXSHADER_H
#define H_APP_DISP_SHADERS_LINEVERTEXSHADER_H

namespace app::disp {

    static const GLchar* g_line_vertex_shader = R"glsl(
    #version 430 core
    in vec2 position;
    in vec3 color;

    out vec3 Color;
    out vec2 normCoord;

    uniform float zoom = 1.0;

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0 / zoom);
        Color = color;
    }
)glsl";

}

#endif // H_APP_DISP_SHADERS_LINEVERTEXSHADER_H
