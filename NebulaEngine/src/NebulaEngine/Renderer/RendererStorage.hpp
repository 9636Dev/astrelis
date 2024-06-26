#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

#include "GraphicsPipeline.hpp"

namespace Nebula
{
    struct Renderer2DStorage
    {
        RefPtr<GraphicsPipeline> m_GraphicsPipeline = nullptr;

        Renderer2DStorage() = default;
    };
} // namespace Nebula
