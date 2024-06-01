#include "Enum.hpp"

namespace Nebula::OpenGL
{
    ClearBufferMask operator|(ClearBufferMask lhs, ClearBufferMask rhs) noexcept
    {
        return static_cast<ClearBufferMask>(static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
    }
} // namespace Nebula::OpenGL
