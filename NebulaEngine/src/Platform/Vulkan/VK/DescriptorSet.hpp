#pragma once

#include "NebulaEngine/Renderer/DescriptorSet.hpp"

#include "DescriptorPool.hpp"
#include "LogicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    struct DescriptorSetInfo
    {
        VkBuffer Buffer;
        std::uint32_t Offset;
        std::uint32_t Size;
        std::uint32_t Binding;
    };

    class DescriptorSet : public Nebula::DescriptorSet
    {
    public:
        DescriptorSet()                                      = default;
        ~DescriptorSet() override                            = default;
        DescriptorSet(const DescriptorSet& other)            = delete;
        DescriptorSet& operator=(const DescriptorSet& other) = delete;
        DescriptorSet(DescriptorSet&& other)                 = delete;
        DescriptorSet& operator=(DescriptorSet&& other)      = delete;

        [[nodiscard]] bool Init(LogicalDevice& device, DescriptorPool& pool, VkDescriptorSetLayout layout, DescriptorSetInfo& info);
        void Destroy(LogicalDevice& device, DescriptorPool& pool) const;

        void Bind(RefPtr<GraphicsContext>& context, RefPtr<Nebula::GraphicsPipeline>& pipeline) const override;

        [[nodiscard]] VkDescriptorSet GetHandle() const { return m_DescriptorSet; }
    private:
        VkDescriptorSet m_DescriptorSet {};
    };
} // namespace Nebula::Vulkan
