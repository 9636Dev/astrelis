#pragma once

#include <vulkan/vulkan.h>

#include "CommandBuffer.hpp"
#include "DescriptorPool.hpp"
#include "DescriptorSetLayout.hpp"
#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan {

    class BindingDescriptorSet;
    class GraphicsPipeline;

    enum class DescriptorType {
        UniformBuffer,
        Sampler
    };

    class DescriptorSet {
    public:
        DescriptorSet()                                      = default;
        ~DescriptorSet()                                     = default;
        DescriptorSet(const DescriptorSet& other)            = default;
        DescriptorSet& operator=(const DescriptorSet& other) = default;
        DescriptorSet(DescriptorSet&& other)                 = default;
        DescriptorSet& operator=(DescriptorSet&& other)      = default;

        [[nodiscard]] bool Init(LogicalDevice& device, DescriptorPool& pool,
            DescriptorSetLayout& layout, const std::vector<DescriptorSetBinding>& descriptors,
            std::uint32_t setIndex);
        void Destroy(LogicalDevice& logicalDevice, DescriptorPool& descriptorPool) const;
        void Bind(CommandBuffer& buffer, GraphicsPipeline& pipeline) const;

        [[nodiscard]] VkDescriptorSet GetHandle() const {
            return m_DescriptorSet;
        }
    private:
        VkDescriptorSet m_DescriptorSet {};
    };
} // namespace Astrelis::Vulkan
