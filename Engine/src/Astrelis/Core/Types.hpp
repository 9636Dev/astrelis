#pragma once

#include <type_traits>

namespace Astrelis
{
    namespace Types
    {
        template<typename T>
        concept Arithmetic = std::is_arithmetic_v<T>;
    } // namespace Types

    using CString = const char*;
} // namespace Astrelis
