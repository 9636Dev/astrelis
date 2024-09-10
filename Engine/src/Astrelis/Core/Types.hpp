#pragma once

#include <type_traits>

namespace Astrelis::Types
{
    template<typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;
} // namespace Astrelis::Types
