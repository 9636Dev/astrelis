#pragma once

#include "NebulaEngine/Core/Math.hpp"

namespace Nebula
{
    struct Material
    {
        Material() = default;
        ~Material() = default;
        Material(const Material &) = default;
        Material &operator=(const Material &) = default;
        Material(Material &&) = default;
        Material &operator=(Material &&) = default;
        bool operator==(const Material &other) const
        {
            return DiffuseColor == other.DiffuseColor;
        }

        Vector4f DiffuseColor = {1.0F, 1.0F, 1.0F, 1.0F};
    };
} // namespace Nebula
