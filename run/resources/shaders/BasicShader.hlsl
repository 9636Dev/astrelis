/*cbuffer VertexBuffer : register(b0)
{
    float4x4 modelMatrix;
};
*/

struct VertexInput
{
    float2 pos : POSITION; // Position of the vertex
};

struct PixelInput
{
    float4 pos : SV_POSITION; // Position for the next stage
};

PixelInput VertexShader(VertexInput input)
{
    PixelInput output;
    // Transform the vertex position into clip space
    //output.pos = mul(modelMatrix, float4(input.pos, 1.0, 1.0));
    output.pos = float4(input.pos, 0.0, 1.0);

    return output;
}

float4 PixelShader(PixelInput input) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
