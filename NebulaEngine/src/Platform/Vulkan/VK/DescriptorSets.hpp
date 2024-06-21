#pragma once

#include "NebulaEngine/Renderer/DescriptorSetLayout.hpp"
#include "NebulaEngine/Renderer/DescriptorSets.hpp"

#include <vulkan/vulkan.h>

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
        DescriptorSets()                                      = default;
        ~DescriptorSets() override                            = default;
        DescriptorSets(const DescriptorSets& other)            = delete;
        DescriptorSets& operator=(const DescriptorSets& other) = delete;
        DescriptorSets(DescriptorSets&& other)                 = delete;
        DescriptorSets& operator=(DescriptorSets&& other)      = delete;

        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context, RefPtr<Nebula::DescriptorSetLayout>& layout, const std::vector<BindingDescriptor>& descriptors);
        void Destroy(RefPtr<GraphicsContext>& context) const;

        void Bind(RefPtr<GraphicsContext>& context, RefPtr<Nebula::GraphicsPipeline>& pipeline) const override;

        [[nodiscard]] VkDescriptorSet GetHandle() const { return m_DescriptorSet; }
    private:
        VkDescriptorSet m_DescriptorSet {};
    };
} // namespace Nebula::Vulkan
