#pragma once

#include "NebulaEngine/Renderer/DescriptorSetLayout.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class DescriptorSetLayout : public Nebula::DescriptorSetLayout
    {
    public:
        DescriptorSetLayout()                                      = default;
        ~DescriptorSetLayout() override                            = default;
        DescriptorSetLayout(const DescriptorSetLayout&)            = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout(DescriptorSetLayout&&)                 = delete;
        DescriptorSetLayout& operator=(DescriptorSetLayout&&)      = delete;

        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context, const std::vector<BindingDescriptor>& descriptors) override;
        void Destroy(RefPtr<GraphicsContext>& context) const override;

        VkDescriptorSetLayout m_Layout = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
