#pragma once

#include "Astrelis/Renderer/BindingDescriptor.hpp"

#include <vulkan/vulkan.h>

#include "CommandBuffer.hpp"
#include "DescriptorPool.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "GraphicsPipeline.hpp"
#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan {
    class BindingDescriptorSet final : public Astrelis::BindingDescriptorSet {
    public:
        explicit BindingDescriptorSet(Mode mode = Mode::PerFrame) : m_Mode(mode) {
        }
        ~BindingDescriptorSet() override                                   = default;
        BindingDescriptorSet(const BindingDescriptorSet& other)            = default;
        BindingDescriptorSet& operator=(const BindingDescriptorSet& other) = default;
        BindingDescriptorSet(BindingDescriptorSet&& other)                 = default;
        BindingDescriptorSet& operator=(BindingDescriptorSet&& other)      = default;

        [[nodiscard]] bool Init(LogicalDevice& device, DescriptorPool& descriptorPool,
            std::uint32_t frames, const std::vector<DescriptorSetBinding>& descriptors);
        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context,
            const std::vector<DescriptorSetBinding>&     descriptors) final;

        void Destroy(LogicalDevice& device, DescriptorPool& descriptorPool) const;
        void Destroy(RefPtr<GraphicsContext>& context) const final;

        void Bind(
            CommandBuffer& commandBuffer, GraphicsPipeline& pipeline, std::uint32_t index) const;
        void Bind(RefPtr<GraphicsContext>&      context,
            RefPtr<Astrelis::GraphicsPipeline>& pipeline) const final;

        DescriptorSetLayout        m_Layout;
        std::vector<DescriptorSet> m_DescriptorSets;
        Mode                       m_Mode;
    };
} // namespace Astrelis::Vulkan
