#include "NebulaShaderCompiler/ShaderCompiler.hpp"
#include <gtest/gtest.h>
#include <iostream>

TEST(Compiler, Compile)
{
    // NOLINTNEXTLINE
    using namespace Nebula::Shader;

    Compiler compiler(R"(
Name "Test";
Bindings {
    Uniforms {
        float4x4 Model : MODEL_MATRIX;
        float4x4 View : VIEW_MATRIX;
        float4x4 Projection : PROJECTION_MATRIX;
    }

    Textures {
        Texture2D Diffuse : DIFFUSE_MAP;
        Texture2D Normal : NORMAL_MAP;
    }

}

Input {
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
}

FragmentInput {
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
}

Shader {
    Shared {
        cbuffer Uniforms : register(b0) {
            float4x4 Model;
            float4x4 View;
            float4x4 Projection;
        }

        Texture2D Diffuse : register(t0);
        Texture2D Normal : register(t1);
        SamplerState Sampler : register(s0);

        struct PixelInput {
            float4 Position : SV_POSITION;
            float3 Normal : NORMAL;
            float2 TexCoord : TEXCOORD;
        }
    }

    Vertex {
        struct VertexInput {
            float3 Position : POSITION;
            float3 Normal : NORMAL;
            float2 TexCoord : TEXCOORD;
        }

        PixelInput main(VertexInput input) {
            PixelInput output;
            output.Position = mul(Model, float4(input.Position, 1.0f));
            output.Position = mul(View, output.Position);
            output.Position = mul(Projection, output.Position);
            output.Normal = input.Normal;
            output.TexCoord = input.TexCoord;
            return output;
        }
    }

    Fragment {
        float4 main(PixelInput input) : SV_TARGET {
            float4 diffuseColor = Diffuse.Sample(Sampler, input.TexCoord);
            float4 normalColor = Normal.Sample(Sampler, input.TexCoord);
            return diffuseColor * normalColor;
        }
    }

    Entrypoint {
        Vertex "main";
        Fragment "main";
    }
}
    )");

    auto error = compiler.Compile();
    if (error.has_value())
    {
        auto [row, column] = compiler.GetRowColumn(error->Index);
        std::cout << "Error: " << error->Message << " at: " << row << ":" << column << std::endl;
    }
    ASSERT_FALSE(error.has_value());

    std::cout << "Name: " << compiler.GetMeta().Name << std::endl;
    for (const auto& uniform : compiler.GetMeta().Uniforms)
    {
        std::cout << "Uniform: " << uniform.Type << " " << uniform.Name << " : " << uniform.Binding << std::endl;
    }

    for (const auto& texture : compiler.GetMeta().Textures)
    {
        std::cout << "Texture: " << texture.Type << " " << texture.Name << " : " << texture.Binding << std::endl;
    }

    for (const auto& input : compiler.GetMeta().Inputs)
    {
        std::cout << "Input: " << input.Type << " " << input.Name << " : " << input.Binding << std::endl;
    }

    for (const auto& fragmentInput : compiler.GetMeta().FragmentInputs)
    {
        std::cout << "FragmentInput: " << fragmentInput.Type << " " << fragmentInput.Name << " : "
                  << fragmentInput.Binding << std::endl;
    }

    std::cout << "Vertex Entrypoint: " << compiler.GetMeta().VertexEntrypoint << std::endl;

    std::cout << "Fragment Entrypoint: " << compiler.GetMeta().FragmentEntrypoint << std::endl;

    std::cout << "Shared: " << compiler.GetSources().Shared.value_or("N/A") << std::endl;

    std::cout << "Vertex: " << compiler.GetSources().Vertex.value_or("N/A") << std::endl;

    std::cout << "Fragment: " << compiler.GetSources().Pixel.value_or("N/A") << std::endl;
}

