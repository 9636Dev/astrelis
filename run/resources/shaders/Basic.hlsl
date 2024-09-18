// Constant buffer for the view and projection matrices
cbuffer UniformBufferObject : register(b0)
{
    float4x4 view;
    float4x4 proj;
};

#if FRAGMENT_SHADER
// Texture sampler and texture (binding t1 for texture and s0 for sampler)
Texture2D textureSampler : register(t1);
SamplerState samplerState : register(s1);
#endif

// Input vertex data
struct VertexIn
{
    float3 position : POSITION;  // Vertex position
    float3 color : COLOR;        // Vertex color
    float2 texcoord : TEXCOORD;  // Texture coordinates
    float4x4 transform : TEXCOORD1; // Transformation matrix
};

// Output data from the vertex shader to the pixel shader
struct VertexOut
{
    float4 position : SV_POSITION; // Clip-space position
    float3 color : COLOR;          // Color passed to pixel shader
    float2 texcoord : TEXCOORD;    // Texture coordinates passed to pixel shader
};

// Vertex shader
VertexOut VS_Main(VertexIn vin)
{
    VertexOut vout;

    // Transform the vertex position by the projection, view, and local transform matrices
    vout.position = mul(proj, mul(view, mul(vin.transform, float4(vin.position, 1.0f))));

    // Pass through the color and texture coordinates
    vout.color = vin.color;
    vout.texcoord = vin.texcoord;

    return vout;
}

#ifdef FRAGMENT_SHADER
// Pixel shader
float4 PS_Main(VertexOut pin) : SV_TARGET
{
    // Sample the texture and multiply it with the color
    float4 texColor = textureSampler.Sample(samplerState, pin.texcoord);
    return float4(pin.color, 1.0f) * texColor;
}

#endif
