#pragma once

#include "Astrelis/Renderer/GraphicsPipeline.hpp"

#include <vulkan/vulkan.h>

#include "DescriptorSetLayout.hpp"
#include "LogicalDevice.hpp"
#include "RenderPass.hpp"

namespace Astrelis::Vulkan
{
    class GraphicsPipeline : public Astrelis::GraphicsPipeline
    {
    public:
        GraphicsPipeline()                                   = default;
        ~GraphicsPipeline() override                         = default;
        GraphicsPipeline(const GraphicsPipeline&)            = delete;
        GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
        GraphicsPipeline(GraphicsPipeline&&)                 = delete;
        GraphicsPipeline& operator=(GraphicsPipeline&&)      = delete;

        [[nodiscard]] bool Init(LogicalDevice& device,
                                VkExtent2D extent,
                                RenderPass& renderPass,
                                PipelineShaders& shaders,
                                std::vector<BufferBinding>& bindings,
                                std::vector<DescriptorSetLayout>& layouts);
        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context,
                                PipelineShaders& shaders,
                                std::vector<Astrelis::BufferBinding>& bindings,
                                std::vector<RefPtr<Astrelis::DescriptorSetLayout>>& layouts,
                                PipelineType type) override;
        void Destroy(RefPtr<GraphicsContext>& context) override;

        void Bind(RefPtr<GraphicsContext>& context) override;

        VkPipeline m_Pipeline             = VK_NULL_HANDLE;
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
