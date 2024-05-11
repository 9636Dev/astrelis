# Nebula Shader Language (.nsl)

## Overview

- **File Extension**: `.nsl`
- **Full Name**: Nebula Shader Language
- **Type**: Text-based
- **Version**: 1.0

## Specification
- **Case Sensitivity**: Case-sensitive
- **Comments**: `//` for single-line comments, `/* */` for multi-line comments
- **Statement Terminator**: `;`

## Example
```nsl
Name "ExampleShader";

Bindings {
    Uniforms "VertexBuffer" {
        float4 Color : TEXTURE_DIFFUSE_COLOR;
    }
    Textures {
        Texture2D Diffuse : TEXTURE_DIFFUSE_MAP;
    }
}

Input {
    float3 Position : POSITION3D;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
}

// Optional (For auto generated Vertex Shader)
PixelInput {
    float3 Position : OUTPUT_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
}

// HLSL Code
Shader {
    // Optional Shared Code
    Shared {
        cbuffer VertexBuffer : register(b0)
        {
            float4 Color;
        };

        Texture2D Diffuse;
    }

    Vertex Generated;

    // Auto generated code, only works for trivial shader passes, will look something like this:
    /*
        struct VertexInput
        {
            float3 Position: POSITION; // Same name as Input
            float3 Normal: NORMAL0;
            float2 TexCoord: TEXCOORD0;
        };

        struct PixelInput
        {
            float3 Position: SV_POSITION;
            float3 Normal: NORMAL0;
            float2 TexCoord: TEXCOORD0;
        };

        PixelInput VertexShader(VertexInput input)
        {
            PixelInput output;

            output.Position = input.Position;
            output.Normal = input.Normal;
            output.TexCoord = input.TexCoord;

            return output;
        }
    */

    Fragment {
        struct PixelInput
        {
            float3 Position : SV_POSITION;
            float3 normal : NORMAL0;
            float2 texCoord : TEXCOORD0;
        };

        float4 PixelShader(PixelInput input) : SV_Target
        {
            // Sample the texture
            float4 texColor = Diffuse.Sample(DiffuseSampler, input.texCoord);

            // Return the color
            return texColor * Color;
        }
    }
}

```




