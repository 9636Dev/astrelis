#version 410
#ifdef GL_ARB_shading_language_420pack
#extension GL_ARB_shading_language_420pack : require
#endif

struct VertexInput
{
    vec3 pos;
    vec4 col;
};

struct PixelInput
{
    vec4 pos;
    vec4 col;
};

layout(binding = 0, std140) uniform type_VertexBuffer
{
    layout(row_major) mat4 mvpMatrix;
} VertexBuffer;

layout(location = 0) in vec3 in_var_POSITION;
layout(location = 1) in vec4 in_var_COLOR;
layout(location = 0) out vec4 out_var_COLOR;

mat4 spvWorkaroundRowMajor(mat4 wrap) { return wrap; }

PixelInput src_main(VertexInput _input)
{
    PixelInput _output;
    _output.pos = vec4(_input.pos, 1.0) * spvWorkaroundRowMajor(VertexBuffer.mvpMatrix);
    _output.col = _input.col;
    return _output;
}

void main()
{
    VertexInput param_var_input = VertexInput(in_var_POSITION, in_var_COLOR);
    PixelInput _30 = src_main(param_var_input);
    gl_Position = _30.pos;
    out_var_COLOR = _30.col;
}

#version 410
#ifdef GL_ARB_shading_language_420pack
#extension GL_ARB_shading_language_420pack : require
#endif

struct VertexInput
{
    vec3 pos;
    vec4 col;
};

struct PixelInput
{
    vec4 pos;
    vec4 col;
};

layout(binding = 0, std140) uniform type_VertexBuffer
{
    layout(row_major) mat4 mvpMatrix;
} VertexBuffer;

layout(location = 0) in vec3 in_var_POSITION;
layout(location = 1) in vec4 in_var_COLOR;
layout(location = 0) out vec4 out_var_COLOR;

mat4 spvWorkaroundRowMajor(mat4 wrap) { return wrap; }

PixelInput src_main(VertexInput _input)
{
    PixelInput _output;
    _output.pos = vec4(_input.pos, 1.0) * spvWorkaroundRowMajor(VertexBuffer.mvpMatrix);
    _output.col = _input.col;
    return _output;
}

void main()
{
    VertexInput param_var_input = VertexInput(in_var_POSITION, in_var_COLOR);
    PixelInput _30 = src_main(param_var_input);
    gl_Position = _30.pos;
    out_var_COLOR = _30.col;
}

