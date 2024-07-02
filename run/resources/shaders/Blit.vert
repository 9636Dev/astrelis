#version 450

layout (location = 0) in vec2 aPos;
layout (location = 0) out vec2 vTex;

void main()
{
    vTex = aPos;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
