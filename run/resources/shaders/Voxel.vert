#version 450

layout (binding = 0) uniform ViewProjection {
    mat4 view;
    mat4 proj;
} ubo;

layout (binding = 1) uniform ChunkUniform {
    ivec3 pos;
} chunk;

// Vertex data
layout (location = 0) in vec3 aPosition;

// Instance data
layout (location = 1) in uint aTranslation;
layout (location = 2) in vec4 aColor;

// Vary
layout (location = 0) out vec4 vColor;

void main() {
    ivec3 translation = ivec3(
        (aTranslation >> 12) & 63,
        (aTranslation >> 6) & 63,
        aTranslation & 53
    );
    ivec3 position = ivec3(
        chunk.pos.x * 64 + translation.x,
        chunk.pos.y * 64 + translation.y,
        chunk.pos.z * 64 + translation.z
    );
    gl_Position = ubo.proj * ubo.view * vec4(aPosition + position, 1.0);
    vColor = aColor;
}
