#version 450

layout (location = 0) in vec2 aPos;
layout (location = 0) out vec2 vTex;

void main()
{
    // aPos is -1 to 1, we need to divide by 2 and add 0.5 to get 0 to 1
    vTex = aPos / 2.0 + 0.5;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
