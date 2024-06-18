#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

#include <vector>

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "Fence.hpp"
#include "GraphicsPipeline.hpp"
#include "ImageViews.hpp"
#include "RenderPass.hpp"
#include "Semaphore.hpp"
#include "SwapChainFrameBuffers.hpp"

namespace Nebula
{
    struct Renderer2DStorage
    {
        RefPtr<CommandPool> m_CommandPool            = nullptr;
        std::vector<RefPtr<CommandBuffer>> m_CommandBuffers;
        RefPtr<ImageViews> m_ImageViews              = nullptr;
        RefPtr<RenderPass> m_RenderPass              = nullptr;
        RefPtr<SwapChainFrameBuffers> m_FrameBuffers = nullptr;
        RefPtr<GraphicsPipeline> m_GraphicsPipeline  = nullptr;
        std::vector<RefPtr<Semaphore>> m_ImageAvailableSemaphores;
        std::vector<RefPtr<Semaphore>> m_RenderFinishedSemaphores;
        std::vector<RefPtr<Fence>> m_InFlightFences;

        Renderer2DStorage() = default;
    };
} // namespace Nebula
