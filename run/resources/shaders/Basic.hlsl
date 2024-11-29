cbuffer UniformBufferObject : register(b0)
{
    row_major float4x4 view;    // View matrix
    row_major float4x4 proj;    // Projection matrix
};

struct VertexIn
{
    float3 position : POSITION;    // Vertex position
    float2 texcoord : TEXCOORD;    // Texture coordinates

    row_major float4x4 transform : TEXCOORD1; // Transformation matrix (model matrix)
    float4 color : COLOR;           // Instance color input
};

struct VertexOut
{
    float4 position : SV_POSITION; // Clip-space position
    float2 texcoord : TEXCOORD;    // Pass-through texture coordinates
    float4 color : COLOR;          // Pass-through instance color
};

// Vertex Shader
VertexOut VS_Main(VertexIn vin)
{
    VertexOut vout;

    // Apply transformations: model (transform) * view * projection
    float4 worldPosition = mul(float4(vin.position, 1.0f), vin.transform);  // Model space to world space
    vout.position = mul(worldPosition, view);                              // World space to view space
    vout.position = mul(vout.position, proj);                              // View space to clip space

    // Pass through texture coordinates and instance color
    vout.texcoord = vin.texcoord;
    vout.color = vin.color;

    return vout;
}

// Pixel Shader
float4 PS_Main(VertexOut pin) : SV_TARGET
{
    // Simple output of the color passed from vertex shader
    return pin.color;
}
