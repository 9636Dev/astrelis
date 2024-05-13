#pragma once

#include "../Math.hpp"

namespace Nebula::Basic
{
    struct VisualProps
    {
        Vector4f Color = Vector4f::Ones();

        VisualProps() = default;
        VisualProps(const VisualProps&) = default;
        VisualProps& operator=(const VisualProps&) = default;
        VisualProps(VisualProps&&) = default;
        VisualProps& operator=(VisualProps&&) = default;
        ~VisualProps() = default;
    };
} // namespace Nebula::Basic
