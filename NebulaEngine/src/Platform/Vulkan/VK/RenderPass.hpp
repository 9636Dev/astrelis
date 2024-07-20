#pragma once

#include <vulkan/vulkan.h>

#include "CommandBuffer.hpp"
#include "LogicalDevice.hpp"

namespace Nebula::Vulkan
{
    struct RenderSubpass
    {
        VkPipelineBindPoint PipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        std::vector<VkAttachmentReference> Attachments;
    };

    struct RenderPassInfo
    {
        std::vector<VkAttachmentDescription> Attachments;
        std::vector<RenderSubpass> Subpasses;

        RenderPassInfo() = default;
    };


    class FrameBuffer;

    class RenderPass
    {
    public:
        RenderPass()                             = default;
        ~RenderPass()                            = default;
        RenderPass(const RenderPass&)            = delete;
        RenderPass& operator=(const RenderPass&) = delete;
        RenderPass(RenderPass&&)                 = delete;
        RenderPass& operator=(RenderPass&&)      = delete;

        [[nodiscard]] bool Init(LogicalDevice& device, RenderPassInfo info = {});
        void Destroy(LogicalDevice& device);

        void Begin(CommandBuffer& commandBuffer, FrameBuffer& frameBuffer, VkExtent2D extent);
        void End(CommandBuffer& buffer);

        [[nodiscard]] VkRenderPass GetHandle() const { return m_RenderPass; }

        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
