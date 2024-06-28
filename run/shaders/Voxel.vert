#version 450

layout (binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in ivec3 aTranslation;

void main() {
    gl_Position = ubo.proj * ubo.view * vec4(aPosition + aTranslation, 1.0);
}
