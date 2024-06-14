#pragma once

#include "NebulaEngine/Core/Math.hpp"

namespace Nebula
{
    struct TransformComponent
    {
        Vector3f Position = {0.0F, 0.0F, 0.0F};
        Quaternionf Rotation = {0.0F, 0.0F, 0.0F, 1.0F};
        Vector3f Scale = {1.0F, 1.0F, 1.0F};
    };
} // namespace Nebula
