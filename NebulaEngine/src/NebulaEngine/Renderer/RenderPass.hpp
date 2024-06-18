#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

#include <cstdint>
#include <vulkan/vulkan.h>

#include "CommandBuffer.hpp"
#include "GraphicsContext.hpp"
#include "SwapChainFrameBuffers.hpp"

namespace Nebula
{
    class RenderPass
    {
    public:
        RenderPass()                             = default;
        virtual ~RenderPass()                    = default;
        RenderPass(const RenderPass&)            = delete;
        RenderPass& operator=(const RenderPass&) = delete;
        RenderPass(RenderPass&&)                 = delete;
        RenderPass& operator=(RenderPass&&)      = delete;

        virtual void Begin([[maybe_unused]] RefPtr<GraphicsContext>& context,
                   [[maybe_unused]] RefPtr<CommandBuffer>& commandBuffer, [[maybe_unused]] RefPtr<SwapChainFrameBuffers>& frameBuffers, [[maybe_unused]] std::uint32_t frameIndex)
        {
        }

        virtual void End([[maybe_unused]] RefPtr<GraphicsContext>& context,
                 [[maybe_unused]] RefPtr<CommandBuffer>& commandBuffer)
        {
        }
    };
} // namespace Nebula

