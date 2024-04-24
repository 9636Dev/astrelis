// VertexShader.hlsl

cbuffer VertexBuffer : register(b0)
{
    float4x4 mvpMatrix; // Model-View-Projection matrix
};

struct VertexInput
{
    float3 pos : POSITION; // Position of the vertex
    float4 col : COLOR;    // Color of the vertex
};

struct PixelInput
{
    float4 pos : SV_POSITION; // Position for the next stage
    float4 col : COLOR;       // Color to pass to the pixel shader
};

PixelInput main(VertexInput input)
{
    PixelInput output;
    // Transform the vertex position into clip space
    output.pos = mul(mvpMatrix, float4(input.pos, 1.0));
    // Pass the color through unmodified
    output.col = input.col;

    return output;
}
