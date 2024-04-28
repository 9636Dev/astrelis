#include "NebulaShaderBuilder/FileFormat.hpp"
#include "NebulaShaderBuilder/FileToOutput.hpp"
#include "NebulaShaderConductor/ShaderOutput.hpp"
#include <gtest/gtest.h>

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

TEST(NebulaShaderTests, ToIntermediate)
{
    auto intermediate = IntermediateFormat::FromSource(source);
    if (!intermediate.second.empty())
    {
        std::cout << "Intemediate File: " << intermediate.second << std::endl;
    }
    ASSERT_TRUE(intermediate.second.empty());

    auto& file = intermediate.first;

    Nebula::ShaderConductor::GLSLOutput output;
    output.Version     = 410;
    std::string result = file.GenerateGlsl(output);
    if (!result.empty())
    {
        std::cout << "Errors: " << result << std::endl;
    }

    // Generate a temp file in the system temp directory
    auto tempFile = std::filesystem::temp_directory_path() / "NebulaShaderBuilderTemp.cnsl"; // 'compiled-nsl' file

    auto aout        = Nebula::File(tempFile);
    auto writeResult = file.WriteToFile(aout);

    ASSERT_TRUE(writeResult.Success);
    ASSERT_GT(writeResult.BytesWritten, 0);
    ASSERT_TRUE(std::filesystem::exists(tempFile));

    auto readResult = IntermediateOutput::FromFile(aout);
    if (readResult.Header.Identifier.empty())
    {
        std::cout << "Failed to read file: " << tempFile << std::endl;
    }

    ASSERT_TRUE(readResult.GlslSource.has_value());
    ASSERT_EQ(readResult.Header.Meta.Name, "BasicShader");
    ASSERT_GT(readResult.Header.Meta.Bindings.size(), 0);
    ASSERT_EQ(readResult.Header.Meta.Textures.size(), 0);
    ASSERT_GT(readResult.GlslSource->VertexSource.size(), 0);
    ASSERT_GT(readResult.GlslSource->PixelSource.size(), 0);

    // Cleanup
    auto res = std::filesystem::remove(tempFile);
    if (!res)
    {
        std::cout << "Failed to remove temp file: " << tempFile << std::endl;
    }
}
