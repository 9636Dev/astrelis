#pragma once

#include "Astrelis/Renderer/DescriptorSets.hpp"

#include <vulkan/vulkan.h>

#include "DescriptorPool.hpp"
#include "DescriptorSetLayout.hpp"
#include "GraphicsPipeline.hpp"
#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan {

    enum class DescriptorType {
        UniformBuffer,
        Sampler
    };

    class DescriptorSets : public Astrelis::DescriptorSets {
    public:
        DescriptorSets()                                       = default;
        ~DescriptorSets() override                             = default;
        DescriptorSets(const DescriptorSets& other)            = default;
        DescriptorSets& operator=(const DescriptorSets& other) = default;
        DescriptorSets(DescriptorSets&& other)                 = default;
        DescriptorSets& operator=(DescriptorSets&& other)      = default;

        bool Init(LogicalDevice& device, DescriptorPool& pool, DescriptorSetLayout& layout,
            const std::vector<BindingDescriptor>& descriptors);
        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context,
            RefPtr<Astrelis::DescriptorSetLayout>&       layout,
            const std::vector<BindingDescriptor>&        descriptors) override;
        void               Destroy(RefPtr<GraphicsContext>& context) const override;

        void Bind(CommandBuffer& buffer, GraphicsPipeline& pipeline) const;
        void Bind(RefPtr<GraphicsContext>&      context,
            RefPtr<Astrelis::GraphicsPipeline>& pipeline) const override;

        [[nodiscard]] VkDescriptorSet GetHandle() const {
            return m_DescriptorSet;
        }
    private:
        VkDescriptorSet m_DescriptorSet {};
    };
} // namespace Astrelis::Vulkan
