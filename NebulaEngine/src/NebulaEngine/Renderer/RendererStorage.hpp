#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

#include <vector>

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "Fence.hpp"
#include "GraphicsPipeline.hpp"
#include "ImageViews.hpp"
#include "IndexBuffer.hpp"
#include "RenderPass.hpp"
#include "Semaphore.hpp"
#include "SwapChainFrameBuffers.hpp"
#include "VertexBuffer.hpp"

namespace Nebula
{
    struct Renderer2DStorage
    {
        RefPtr<CommandPool> m_CommandPool = nullptr;
        std::vector<RefPtr<CommandBuffer>> m_CommandBuffers;
        RefPtr<ImageViews> m_ImageViews              = nullptr;
        RefPtr<RenderPass> m_RenderPass              = nullptr;
        RefPtr<SwapChainFrameBuffers> m_FrameBuffers = nullptr;
        RefPtr<GraphicsPipeline> m_GraphicsPipeline  = nullptr;
        RefPtr<VertexBuffer> m_VertexBuffer          = nullptr;
        RefPtr<IndexBuffer> m_IndexBuffer            = nullptr;
        std::vector<RefPtr<Semaphore>> m_ImageAvailableSemaphores;
        std::vector<RefPtr<Semaphore>> m_RenderFinishedSemaphores;
        std::vector<RefPtr<Fence>> m_InFlightFences;
        std::uint32_t m_CurrentFrame = 0;

        Renderer2DStorage() = default;
    };
} // namespace Nebula
