#pragma once

#include <cstdint>

namespace Nebula
{
    struct Bounds
    {
        std::int32_t X;
        std::int32_t Y;
        std::int32_t Width;
        std::int32_t Height;

        constexpr Bounds(std::int32_t paramX, std::int32_t paramY, std::int32_t width, std::int32_t height) noexcept :
            X(paramX),
            Y(paramY),
            Width(width),
            Height(height)
        {
        }

        Bounds()                         = default;
        Bounds(const Bounds&)            = default;
        Bounds& operator=(const Bounds&) = default;
        Bounds(Bounds&&)                 = default;
        Bounds& operator=(Bounds&&)      = default;
        ~Bounds()                        = default;
    };
} // namespace Nebula
