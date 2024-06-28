#version 450

layout (binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout(location = 3) in mat4 transform;
// This takes up 3,4,5,6

layout (location = 0) out vec3 vColor;
layout (location = 1) out vec2 vTexCoord;

void main() {
    gl_Position = ubo.proj * ubo.view * transform * vec4(aPosition, 1.0);
    vColor = aColor;
    vTexCoord = aTexCoord;
}
