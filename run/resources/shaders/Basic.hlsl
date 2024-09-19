// Constant buffer for the view and projection matrices
cbuffer UniformBufferObject : register(b0)
{
    column_major float4x4 view;
    column_major float4x4 proj;
};

// Input vertex data
struct VertexIn
{
    float3 position : POSITION;  // Vertex position
    float2 texcoord : TEXCOORD;  // Texture coordinates
    column_major float4x4 transform : TEXCOORD1; // Transformation matrix
    float4 color : COLOR;  // Instance color input
};

// Output data from the vertex shader to the pixel shader
struct VertexOut
{
    float4 position : SV_POSITION; // Clip-space position
    float2 texcoord : TEXCOORD;    // Texture coordinates passed to pixel shader
    float4 color : COLOR;          // Color passed to pixel shader
};

// Vertex shader
VertexOut VS_Main(VertexIn vin)
{
    VertexOut vout;

    // Transform the vertex position by the projection, view, and local transform matrices
    vout.position = mul(proj, mul(view, mul(vin.transform, float4(vin.position, 1.0f))));

    // Pass through the color and texture coordinates
    vout.texcoord = vin.texcoord;
    vout.color = vin.color; // Pass instance color

    return vout;
}

// Pixel shader
float4 PS_Main(VertexOut pin) : SV_TARGET
{
    return pin.color;
}

