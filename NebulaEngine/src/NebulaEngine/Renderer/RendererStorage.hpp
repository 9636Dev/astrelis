#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

#include <vector>

#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "GraphicsPipeline.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "VertexBuffer.hpp"

namespace Nebula
{
    struct Renderer2DStorage
    {
        RefPtr<GraphicsPipeline> m_GraphicsPipeline       = nullptr;
        RefPtr<VertexBuffer> m_VertexBuffer               = nullptr;
        RefPtr<IndexBuffer> m_IndexBuffer                 = nullptr;
        RefPtr<DescriptorSetLayout> m_DescriptorSetLayout = nullptr;
        std::vector<RefPtr<UniformBuffer>> m_UniformBuffers;
        std::vector<RefPtr<DescriptorSet>> m_DescriptorSets;

        Renderer2DStorage() = default;
    };
} // namespace Nebula
