#pragma once

#include <type_traits>

namespace Nebula::Types
{
    template<typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;
} // namespace Nebula::Types
