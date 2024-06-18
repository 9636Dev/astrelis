#pragma once

#include "NebulaEngine/Renderer/SwapChainFrameBuffers.hpp"

#include "LogicalDevice.hpp"
#include "RenderPass.hpp"
#include "ImageViews.hpp"
#include "SwapChain.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class SwapChainFrameBuffers : public Nebula::SwapChainFrameBuffers
    {
    public:
        SwapChainFrameBuffers() = default;
        ~SwapChainFrameBuffers() override = default;
        SwapChainFrameBuffers(const SwapChainFrameBuffers &) = delete;
        SwapChainFrameBuffers &operator=(const SwapChainFrameBuffers &) = delete;
        SwapChainFrameBuffers(SwapChainFrameBuffers &&) = delete;
        SwapChainFrameBuffers &operator=(SwapChainFrameBuffers &&) = delete;

        bool Init(LogicalDevice& device, RenderPass& renderPass, ImageViews& imageViews, SwapChain& swapChain);
        void Destroy(LogicalDevice& device);

        std::vector<VkFramebuffer> m_SwapChainFrameBuffers;
    };
} // namespace Nebula::Vulkan
