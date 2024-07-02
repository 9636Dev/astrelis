#version 450

layout (location = 0) in vec2 vTex;
layout (location = 0) out vec4 oColor;

layout (binding = 0) uniform sampler2D graphicsSampler;
layout (binding = 1) uniform sampler2D uiSampler;

void main()
{
    vec4 texel0 = texture(graphicsSampler, vTex);
    vec4 texel1 = texture(uiSampler, vTex);
    oColor = mix(texel0, texel1, texel1.a);
}
