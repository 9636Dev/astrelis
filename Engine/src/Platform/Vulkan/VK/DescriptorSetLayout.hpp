#pragma once

#include "Astrelis/Renderer/DescriptorSetLayout.hpp"

#include <vulkan/vulkan.h>

#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan {
    class DescriptorSetLayout : public Astrelis::DescriptorSetLayout {
    public:
        DescriptorSetLayout()                                      = default;
        ~DescriptorSetLayout() override                            = default;
        DescriptorSetLayout(const DescriptorSetLayout&)            = default;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = default;
        DescriptorSetLayout(DescriptorSetLayout&&)                 = default;
        DescriptorSetLayout& operator=(DescriptorSetLayout&&)      = default;

        [[nodiscard]] bool Init(
            LogicalDevice& device, const std::vector<BindingDescriptor>& descriptors);
        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context,
            const std::vector<BindingDescriptor>&        descriptors) override;
        void               Destroy(LogicalDevice& device) const;
        void               Destroy(RefPtr<GraphicsContext>& context) const override;

        VkDescriptorSetLayout m_Layout = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
