#pragma once

#include <cstdint>
#include <type_traits>

namespace Nebula::OpenGL
{
    enum class ClearTarget : std::uint32_t
    {
        Color   = 0x00004000,
        Depth   = 0x00000100,
        Stencil = 0x00000400
    };

    ClearTarget operator|(ClearTarget lhs, ClearTarget rhs);

    enum class BufferTarget : std::uint32_t
    {
        ArrayBuffer = 0x8892,
        ElementArrayBuffer = 0x8893
    };

    enum class BufferUsage : std::uint32_t
    {
        StreamDraw = 0x88E0,
        StreamRead = 0x88E1,
        StreamCopy = 0x88E2,
        StaticDraw = 0x88E4,
        StaticRead = 0x88E5,
        StaticCopy = 0x88E6,
        DynamicDraw = 0x88E8,
        DynamicRead = 0x88E9,
        DynamicCopy = 0x88EA
    };

    enum class DrawMode : std::uint32_t
    {
        Points = 0x0000,
        Lines = 0x0001,
        LineLoop = 0x0002,
        LineStrip = 0x0003,
        Triangles = 0x0004,
        TriangleStrip = 0x0005,
        TriangleFan = 0x0006
    };

    enum class ShaderType : std::uint32_t
    {
        Vertex = 0x8B31,
        Fragment = 0x8B30
    };

    enum class ShaderParameter : std::uint32_t
    {
        CompileStatus = 0x8B81,
        InfoLogLength = 0x8B84
    };

    enum class ProgramParameter : std::uint32_t
    {
        LinkStatus = 0x8B82,
        InfoLogLength = 0x8B84
    };

    enum class GLType : std::uint32_t
    {
        UnsignedByte = 0x1401,
        UnsignedShort = 0x1403,
        UnsignedInt = 0x1405,
        Float = 0x1406
    };

    template<typename T>
    constexpr GLType GetGLType()
    {
        if constexpr (std::is_same_v<T, std::uint8_t>)
        {
            return GLType::UnsignedByte;
        }
        else if constexpr (std::is_same_v<T, std::uint16_t>)
        {
            return GLType::UnsignedShort;
        }
        else if constexpr (std::is_same_v<T, std::uint32_t>)
        {
            return GLType::UnsignedInt;
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            return GLType::Float;
        }
        else
        {
            static_assert(false, "Unsupported type");
        }
    }

    constexpr std::uint32_t GetGLTypeSize(GLType type)
    {
        switch (type)
        {
        case GLType::UnsignedByte:
            return sizeof(std::uint8_t);
        case GLType::UnsignedShort:
            return sizeof(std::uint16_t);
        case GLType::UnsignedInt:
            return sizeof(std::uint32_t);
        case GLType::Float:
            return sizeof(float);
        default:
            return 0;
        }
    }
} // namespace Nebula::OpenGL
