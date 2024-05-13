#pragma once

#include <string>

namespace Nebula::OpenGLDefaults
{
    inline std::string DefaultVertexShader()
    {
        return R"(#version 410
            layout (std140) uniform Camera
            {
                mat4 view;
                mat4 projection;
            };

            layout (location = 0) in vec3 inPosition;
            layout (location = 1) in vec2 inTexCoord;

            layout (location = 2) in vec4 inModelMatrix0;
            layout (location = 3) in vec4 inModelMatrix1;
            layout (location = 4) in vec4 inModelMatrix2;
            layout (location = 5) in vec4 inModelMatrix3;
            layout (location = 6) in vec4 inDiffuseColor;

            layout (location = 0) out vec2 varyTexCoord;
            layout (location = 1) out vec4 varyDiffuseColor;

            void main()
            {
                varyTexCoord = inTexCoord;
                varyDiffuseColor = inDiffuseColor;

                mat4 modelMatrix = mat4(inModelMatrix0, inModelMatrix1, inModelMatrix2, inModelMatrix3);

                // We have row-major matrices, so we reverse the order of the multiplication
                gl_Position = vec4(inPosition, 1.0) * modelMatrix * view * projection;

            }
        )";
    }

    inline std::string DefaultFragmentShader()
    {
        return R"(#version 410
            layout (location = 0) in vec2 varyTexCoord;
            layout (location = 1) in vec4 varyDiffuseColor;

            layout (location = 0) out vec4 outColor;

            uniform sampler2D texture;

            void main()
            {
                outColor = texture(texture, varyTexCoord) * varyDiffuseColor;
            }
        )";
    }
} // namespace Nebula::OpenGLDefaults
