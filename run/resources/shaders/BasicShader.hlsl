cbuffer VertexBuffer : register(b0)
{
    float4x4 modelMatrix;
};

struct VertexInput
{
    float2 pos : POSITION;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
};

PixelInput VertexShader(VertexInput input)
{
    PixelInput output;
    output.pos = mul(modelMatrix, float4(input.pos, 0.0, 1.0));

    return output;
}

float4 PixelShader(PixelInput input) : SV_Target
{
    return float4(1.0, 1.0, 1.0, 1.0);
}
