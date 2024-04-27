#pragma once

#include <array>

namespace TestSources
{

    constexpr auto vertexEntry = "VSMain";
    constexpr auto pixelEntry  = "PSMain";

    constexpr std::array<const char*, 2> hlslSources = {
        R"(
cbuffer Constants : register(b0)
{
    float4x4 worldViewProj;
};

struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    output.position = mul(float4(input.position, 1.0f), worldViewProj);
    output.color = input.color;
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}
)",
        R"(
cbuffer Constants : register(b0)
{
    float4x4 worldViewProj;
};

Texture2D texture0 : register(t0);
sampler sampler0 : register(s0);

struct VSInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    output.position = mul(float4(input.position, 1.0f), worldViewProj);
    output.texcoord = input.texcoord;
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return texture0.Sample(sampler0, input.texcoord);
}
)"};
} // namespace TestSources
