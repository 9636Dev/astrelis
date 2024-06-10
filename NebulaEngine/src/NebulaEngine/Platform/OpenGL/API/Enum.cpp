#include "Enum.hpp"
#include <type_traits>

namespace Nebula::OpenGL
{
    ClearTarget operator|(ClearTarget lhs, ClearTarget rhs)
    {
        return static_cast<ClearTarget>(static_cast<std::underlying_type_t<ClearTarget>>(lhs) | static_cast<std::underlying_type_t<ClearTarget>>(rhs));
    }
} // namespace Nebula::OpenGL
