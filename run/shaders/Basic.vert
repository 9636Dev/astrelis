#version 450

layout (binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

layout (location = 0) out vec3 vColor;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(aPosition, 1.0);
    vColor = aColor;
}
