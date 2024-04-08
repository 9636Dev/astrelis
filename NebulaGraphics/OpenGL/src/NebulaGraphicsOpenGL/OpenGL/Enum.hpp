#pragma once

#include <NebulaCore/Assert.hpp>
#include <cstdint>

namespace Nebula::OpenGL
{
    enum class EnableTarget : std::uint32_t
    {
        Blend = 0x0BE2,
    };

    enum class BlendFactor : std::uint32_t
    {
        SourceAlpha         = 0x0302,
        OneMinusSourceAlpha = 0x0303,

    };

    enum class PolygonFace : std::uint32_t
    {
        Front        = 0x0404,
        Back         = 0x0405,
        FrontAndBack = 0x0408
    };

    enum class PolygonMode : std::uint32_t
    {
        Point = 0x1B00,
        Line  = 0x1B01,
        Fill  = 0x1B02
    };

    enum class TextureWrap : std::uint32_t
    {
        Repeat         = 0x2901,
        MirroredRepeat = 0x8370,
        ClampToEdge    = 0x812F,
        ClampToBorder  = 0x812D
    };

    enum class TextureUnit : std::uint32_t
    {
        Texture0  = 0x84C0,
        Texture1  = 0x84C1,
        Texture2  = 0x84C2,
        Texture3  = 0x84C3,
        Texture4  = 0x84C4,
        Texture5  = 0x84C5,
        Texture6  = 0x84C6,
        Texture7  = 0x84C7,
        Texture8  = 0x84C8,
        Texture9  = 0x84C9,
        Texture10 = 0x84CA,
        Texture11 = 0x84CB,
        Texture12 = 0x84CC,
        Texture13 = 0x84CD,
        Texture14 = 0x84CE,
        Texture15 = 0x84CF,
    };

    static TextureUnit GetTextureUnit(std::uint32_t unit)
    {
        NEB_ASSERT(unit < 16, "Texture unit must be between 0 and 15!");
        return static_cast<TextureUnit>(static_cast<std::uint32_t>(TextureUnit::Texture0) + unit);
    }

    enum class TextureFilter : std::uint32_t
    {
        Nearest              = 0x2600,
        Linear               = 0x2601,
        NearestMipmapNearest = 0x2700,
        LinearMipmapNearest  = 0x2701,
        NearestMipmapLinear  = 0x2702,
        LinearMipmapLinear   = 0x2703
    };

    enum class GetParameterI : std::uint32_t
    {
        VertexArrayBinding           = 0x85B5,
        ArrayBufferBinding           = 0x8894,
        ElementArrayBufferBinding    = 0x8895,
        MaxVertexAttribs             = 0x8869,
        CurrentProgram               = 0x8B8D,
        FrameBufferBinding           = 0x8CA6,
        MaxCombinedTextureImageUnits = 0x8B4D,
    };

    enum class DrawMode : std::uint32_t
    {
        Points        = 0x0000,
        Lines         = 0x0001,
        LineLoop      = 0x0002,
        LineStrip     = 0x0003,
        Triangles     = 0x0004,
        TriangleStrip = 0x0005,
        TriangleFan   = 0x0006
    };

    enum class TextureParameter : std::uint32_t
    {
        DepthStencilTextureMode = 0x90EA,
        TextureBaseLevel        = 0x813C,
        TextureCompareFunc      = 0x884D,
        TextureCompareMode      = 0x884C,
        TextureLodBias          = 0x8501,
        TextureMinFilter        = 0x2801,
        TextureMagFilter        = 0x2800,
        TextureMinLod           = 0x813A,
        TextureMaxLod           = 0x813B,
        TextureMaxLevel         = 0x813D,
        TextureSwizzleR         = 0x8E42,
        TextureSwizzleG         = 0x8E43,
        TextureSwizzleB         = 0x8E44,
        TextureSwizzleA         = 0x8E45,
        TextureWrapS            = 0x2802,
        TextureWrapT            = 0x2803,
        TextureWrapR            = 0x8072
    };

    enum class InternalFormat : std::uint32_t
    {
        DepthComponent = 0x1902,
        DepthStencil   = 0x84F9,
        Red            = 0x1903,
        RG             = 0x8227,
        RGB            = 0x1907,
        RGBA           = 0x1908,
        R8             = 0x8229,
        R8_SNorm       = 0x8F94,
        R16            = 0x822A,
        R16_SNorm      = 0x8F98,
        RG8            = 0x822B,
        RG8_SNorm      = 0x8F95,
        RG16           = 0x822C,
        RG16_SNorm     = 0x8F99,
        R3_G3_B2       = 0x2A10,
        RGB4           = 0x804F,
        RGB5           = 0x8050,
        RGB8           = 0x8051,
        RGB8_SNORM     = 0x8F96,
        RGB10          = 0x8052,
        RGB12          = 0x8053,
        RGB16_SNORM    = 0x8F9A,
        RGBA2          = 0x8055,
        RGBA4          = 0x8056,
        RGB5_A1        = 0x8057,
        RGBA8          = 0x8058,
        RGBA8_SNORM    = 0x8F97,
        RGB10_A2       = 0x8059,
        RGB10_A2UI     = 0x906F,
        RGBA12         = 0x805A,
        RGBA16         = 0x805B,
        SRGB8          = 0x8C41,
        SRGB8_ALPHA8   = 0x8C43,
        R16F           = 0x822D,
        RG16F          = 0x822F,
        RGB16F         = 0x881B,
        RGBA16F        = 0x881A,
        R32F           = 0x822E,
        RG32F          = 0x8230
    };

    enum class PixelFormat : std::uint32_t
    {
        DepthComponent = 0x1902,
        DepthStencil   = 0x84F9,
        Red            = 0x1903,
        RG             = 0x8227,
        RGB            = 0x1907,
        RGBA           = 0x1908,
        BGR            = 0x80E0,
        BGRA           = 0x80E1,
        RedInteger     = 0x8D94,
        RGInteger      = 0x8228,
        RGBInteger     = 0x8D98,
        RGBAInteger    = 0x8D99,
        BGRInteger     = 0x8D9A,
        BGRAInteger    = 0x8D9B,
        StencilIndex   = 0x1901
    };

    enum class PixelType : std::uint32_t
    {
        UnsignedByte              = 0x1401,
        Byte                      = 0x1400,
        UnsignedShort             = 0x1403,
        Short                     = 0x1402,
        UnsignedInt               = 0x1405,
        Int                       = 0x1404,
        Float                     = 0x1406,
        UnsignedByte_3_3_2        = 0x8032,
        UnsignedByte_2_3_3_REV    = 0x8362,
        UnsignedShort_5_6_5       = 0x8363,
        UnsignedShort5_6_5_REV    = 0x8364,
        UnsignedShort4_4_4_4      = 0x8033,
        UnsignedShort4_4_4_4_REV  = 0x8365,
        UnsignedShort5_5_5_1      = 0x8034,
        UnsignedShort1_5_5_5_REV  = 0x8366,
        UnsignedInt8_8_8_8        = 0x8035,
        UnsignedInt8_8_8_8_REV    = 0x8367,
        UnsignedInt10_10_10_2     = 0x8036,
        UnsignedInt2_10_10_10_REV = 0x8368
    };

    enum class TextureTarget : std::uint32_t
    {
        Texture1D                 = 0x0DE0,
        Texture2D                 = 0x0DE1,
        Texture3D                 = 0x806F,
        Texture1DArray            = 0x8C18,
        Texture2DArray            = 0x8C1A,
        TextureRectangle          = 0x84F5,
        TextureCubeMap            = 0x8513,
        TextureCubeMapArray       = 0x9009,
        TextureBuffer             = 0x8C2A,
        Texture2DMultisample      = 0x9100,
        Texture2DMultisampleArray = 0x9102
    };

    enum class BufferTarget : std::uint32_t
    {
        ArrayBuffer             = 0x8892,
        AtomicCounterBuffer     = 0x92C0,
        CopyReadBuffer          = 0x8F36,
        CopyWriteBuffer         = 0x8F37,
        DrawIndirectBuffer      = 0x8F3F,
        DispatchIndirectBuffer  = 0x90EE,
        ElementArrayBuffer      = 0x8893,
        PixelPackBuffer         = 0x88EB,
        PixelUnpackBuffer       = 0x88EC,
        QueryBuffer             = 0x9192,
        ShaderStorageBuffer     = 0x90D2,
        TextureBuffer           = 0x8C2A,
        TransformFeedbackBuffer = 0x8C8E,
        UniformBuffer           = 0x8A11
    };

    enum class BufferUsage : std::uint32_t
    {
        StraemDraw  = 0x88E0,
        StreamRead  = 0x88E1,
        StreamCopy  = 0x88E2,
        StaticDraw  = 0x88E4,
        StaticRead  = 0x88E5,
        StaticCopy  = 0x88E6,
        DynamicDraw = 0x88E8,
        DynamicRead = 0x88E9,
        DYnamicCopy = 0x88EA
    };

    enum class ShaderType : std::uint32_t
    {
        VertexShader       = 0x8B31,
        FragmentShader     = 0x8B30,
        GeometryShader     = 0x8DD9,
        TessControlShader  = 0x8E88,
        TessEvaluateShader = 0x8E87,
        ComputeShader      = 0x91B9
    };

    enum class Type : std::uint32_t
    {
        Byte          = 0x1400,
        UnsignedByte  = 0x1401,
        Short         = 0x1402,
        UnsignedShort = 0x1403,
        Int           = 0x1404,
        UnsignedInt   = 0x1405,
        Float         = 0x1406,
        Fixed         = 0x140C,
        Double        = 0x140A,
        HalfFloat     = 0x140B
    };

    enum class ShaderParameter : std::uint32_t
    {
        DeleteStatus       = 0x8B80,
        CompileStatus      = 0x8B81,
        InfoLogLength      = 0x8B84,
        ShaderSourceLength = 0x8B88
    };

    enum class ProgramParameter : std::uint32_t
    {
        DeleteStatus             = 0x8B80,
        LinkStatus               = 0x8B82,
        ValidateStatus           = 0x8B83,
        InfoLogLength            = 0x8B84,
        AttachedShaders          = 0x8B85,
        ActiveAttributes         = 0x8B89,
        ActiveAttributeMaxLength = 0x8B8A,
        ActiveUniforms           = 0x8B86
    };

    enum class ClearTarget : std::uint32_t
    {
        ColorBufferBit   = 0x00004000,
        DepthBufferBit   = 0x00000100,
        StencilBufferBit = 0x00000400,
    };

    inline ClearTarget operator|(ClearTarget lhs, ClearTarget rhs)
    {
        return static_cast<ClearTarget>(static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
    }
} // namespace Nebula::OpenGL
