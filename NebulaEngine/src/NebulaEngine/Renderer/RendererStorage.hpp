#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

#include <unordered_map>
#include <vector>

#include "GraphicsPipeline.hpp"
#include "IndexBuffer.hpp"

namespace Nebula
{
    struct Renderer2DStorage
    {
        RefPtr<GraphicsPipeline> m_GraphicsPipeline = nullptr;
        RefPtr<IndexBuffer> m_IndexBuffer           = nullptr;

        Renderer2DStorage() = default;
    };
} // namespace Nebula
