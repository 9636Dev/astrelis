#pragma once

#include "NebulaEngine/Renderer/RenderPass.hpp"

#include "Platform/Vulkan/VK/LogicalDevice.hpp"
#include "Platform/Vulkan/VK/SwapChain.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    struct RenderPassInfo
    {
        VkAttachmentLoadOp loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
        VkAttachmentStoreOp storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        VkAttachmentLoadOp stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        RenderPassInfo() = default;
    };

    class RenderPass : public Nebula::RenderPass
    {
    public:
        RenderPass()                             = default;
        ~RenderPass() override                   = default;
        RenderPass(const RenderPass&)            = delete;
        RenderPass& operator=(const RenderPass&) = delete;
        RenderPass(RenderPass&&)                 = delete;
        RenderPass& operator=(RenderPass&&)      = delete;

        bool Init(LogicalDevice& device, SwapChain& swapChain, RenderPassInfo info = {});
        void Destroy(LogicalDevice& device);

        void Begin(RefPtr<GraphicsContext>& context,
                   RefPtr<Nebula::CommandBuffer>& commandBuffer,
                   RefPtr<Nebula::SwapChainFrameBuffers>& frameBuffers,
                   std::uint32_t frameIndex) override;
        void End(RefPtr<GraphicsContext>& context, RefPtr<Nebula::CommandBuffer>& commandBuffer) override;

        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
