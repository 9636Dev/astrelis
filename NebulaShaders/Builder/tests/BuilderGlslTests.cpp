#include <gtest/gtest.h>
#include "NebulaShaderBuilder/FileFormat.hpp"
#include "NebulaShaderConductor/ShaderOutput.hpp"

using namespace Nebula::Shader; // NOLINT

constexpr auto source = R"(
Name "BasicShader";

Bindings {
    Uniforms {
        float4x4 Model : MODEL_MATRIX;
    }
    Textures {}
}

Input {
    float2 Position : POSITION;
}

PixelInput {
    float4 Position : OUTPUT_POSITION;
}

// HLSL Code
Shader {
    Shared {
        cbuffer VertexBuffer : register(b0)
        {
            float4x4 modelMatrix;
        };

        struct PixelInput
        {
            float4 pos : SV_POSITION;
        };
    }

    Vertex {
        struct VertexInput
        {
            float2 pos : POSITION;
        };

        PixelInput VertexShader(VertexInput input)
        {
            PixelInput output;
            output.pos = float4(input.pos, 0.0, 1.0);
            return output;
        }
    }

    Fragment {
        float4 PixelShader(PixelInput input) : SV_TARGET
        {
            return float4(1.0, 1.0, 1.0, 1.0);
        }
    }

    Entrypoint {
        Vertex "VertexShader";
        Fragment "PixelShader";
    }
}
)";

TEST(NebulaShaderTests, ToIntermediate) {
    auto intermediate = IntermediateFormat::FromSource(source);
    if (!intermediate.second.empty()) {
        std::cout << "Intemediate File: " << intermediate.second << std::endl;
    }
    ASSERT_TRUE(intermediate.second.empty());

    auto& file = intermediate.first;

    Nebula::ShaderConductor::GLSLOutput output;
    output.Version = 410;
    std::string result = file.GenerateGlsl(output);
    if (!result.empty()) {
        std::cout << "Errors: " << result << std::endl;
    }

    // Write to a.out
    /*auto aout = Nebula::File::FromPathString("a.out");
    auto writeResult = file.WriteToFile(aout);

    ASSERT_TRUE(writeResult.Success);
    ASSERT_GT(writeResult.BytesWritten, 0); */
}
