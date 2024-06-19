#pragma once

#include "NebulaEngine/Renderer/GraphicsPipeline.hpp"

#include "LogicalDevice.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class GraphicsPipeline : public Nebula::GraphicsPipeline
    {
    public:
        GraphicsPipeline()                                   = default;
        ~GraphicsPipeline() override                         = default;
        GraphicsPipeline(const GraphicsPipeline&)            = delete;
        GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
        GraphicsPipeline(GraphicsPipeline&&)                 = delete;
        GraphicsPipeline& operator=(GraphicsPipeline&&)      = delete;

        bool Init(LogicalDevice& device, RenderPass& renderPass, SwapChain& swapChain, VertexInput& vertexInput);
        void Destroy(LogicalDevice& device);

        void Bind(RefPtr<Nebula::CommandBuffer>& commandBuffer) override;

        VkPipeline m_Pipeline             = VK_NULL_HANDLE;
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
