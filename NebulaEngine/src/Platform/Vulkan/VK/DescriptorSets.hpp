#pragma once

#include <vulkan/vulkan.h>

#include "DescriptorPool.hpp"
#include "DescriptorSetLayout.hpp"
#include "GraphicsPipeline.hpp"
#include "LogicalDevice.hpp"

#include "NebulaEngine/Renderer/DescriptorSets.hpp"

namespace Nebula::Vulkan
{

    enum class DescriptorType
    {
        UniformBuffer,
        Sampler
    };

    class DescriptorSets : public Nebula::DescriptorSets
    {
    public:
        DescriptorSets()                                       = default;
        ~DescriptorSets() override                             = default;
        DescriptorSets(const DescriptorSets& other)            = delete;
        DescriptorSets& operator=(const DescriptorSets& other) = delete;
        DescriptorSets(DescriptorSets&& other)                 = delete;
        DescriptorSets& operator=(DescriptorSets&& other)      = delete;

        bool Init(LogicalDevice& device,
                  DescriptorPool& pool,
                  DescriptorSetLayout& layout,
                  const std::vector<BindingDescriptor>& descriptors);
        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context,
                                RefPtr<Nebula::DescriptorSetLayout>& layout,
                                const std::vector<BindingDescriptor>& descriptors) override;
        void Destroy(RefPtr<GraphicsContext>& context) const override;

        void Bind(CommandBuffer& buffer, GraphicsPipeline& pipeline) const;
        void Bind(RefPtr<GraphicsContext>& context, RefPtr<Nebula::GraphicsPipeline>& pipeline) const override;

        [[nodiscard]] VkDescriptorSet GetHandle() const { return m_DescriptorSet; }
    private:
        VkDescriptorSet m_DescriptorSet {};
    };
} // namespace Nebula::Vulkan
