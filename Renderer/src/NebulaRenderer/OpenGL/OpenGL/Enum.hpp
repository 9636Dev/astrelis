#pragma once

#include <cstdint>

namespace Nebula::OpenGL
{
    enum class GetTarget : std::uint32_t
    {
        MajorVersion = 0x821B,
        MinorVersion = 0x821C,
    };

    enum class ClearBufferMask : std::uint32_t
    {
        ColorBufferBit   = 0x00004000,
        DepthBufferBit   = 0x00000100,
        StencilBufferBit = 0x00000400,
    };

    ClearBufferMask operator|(ClearBufferMask lhs, ClearBufferMask rhs) noexcept;

} // namespace Nebula::OpenGL
