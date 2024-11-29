#include "BindingDescriptorSet.hpp"

#include "Astrelis/Core/Base.hpp"

#include "GraphicsPipeline.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Astrelis::Vulkan {
    bool BindingDescriptorSet::Init(LogicalDevice& device, DescriptorPool& descriptorPool,
        std::uint32_t sets, const std::vector<DescriptorSetBinding>& descriptors) {
        if (!m_Layout.Init(device, descriptors)) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create descriptor set layout.");
            return false;
        }

        m_DescriptorSets.resize(sets);
        for (std::uint32_t i = 0; i < sets; ++i) {
            if (!m_DescriptorSets[i].Init(device, descriptorPool, m_Layout, descriptors, i)) {
                ASTRELIS_CORE_LOG_ERROR("Failed to create descriptor set.");
                return false;
            }
        }

        return true;
    }
    bool BindingDescriptorSet::Init(
        RefPtr<GraphicsContext>& context, const std::vector<DescriptorSetBinding>& descriptors) {
        auto          ctx = context.As<VulkanGraphicsContext>();
        std::uint32_t sets =
            m_Mode == Mode::One ? 1 : static_cast<std::uint32_t>(ctx->m_Frames.size());
        return Init(ctx->m_LogicalDevice, ctx->m_DescriptorPool, sets, descriptors);
    }

    void BindingDescriptorSet::Destroy(
        LogicalDevice& device, DescriptorPool& descriptorPool) const {
        for (const auto& descriptorSet : m_DescriptorSets) {
            descriptorSet.Destroy(device, descriptorPool);
        }

        m_Layout.Destroy(device);
    }
    void BindingDescriptorSet::Destroy(RefPtr<GraphicsContext>& context) const {
        auto ctx = context.As<VulkanGraphicsContext>();
        Destroy(ctx->m_LogicalDevice, ctx->m_DescriptorPool);
    }

    void BindingDescriptorSet::Bind(
        CommandBuffer& commandBuffer, GraphicsPipeline& pipeline, std::uint32_t index) const {
        m_DescriptorSets[index].Bind(commandBuffer, pipeline);
    }

    void BindingDescriptorSet::Bind(
        RefPtr<GraphicsContext>& context, RefPtr<Astrelis::GraphicsPipeline>& pipeline) const {
        auto ctx = context.As<VulkanGraphicsContext>();
        Bind(ctx->GetCurrentFrame().CommandBuffer, *(pipeline.As<GraphicsPipeline>()),
            m_Mode == Mode::One ? 0 : ctx->GetCurrentFrameIndex());
    }
} // namespace Astrelis::Vulkan
