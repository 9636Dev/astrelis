#pragma once

#include "NebulaEngine/Renderer/DescriptorSet.hpp"

#include "LogicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class DescriptorSet : public Nebula::DescriptorSet
    {
    public:
        DescriptorSet();
        ~DescriptorSet() override;
        DescriptorSet(const DescriptorSet &other) = delete;
        DescriptorSet &operator=(const DescriptorSet &other) = delete;
        DescriptorSet(DescriptorSet &&other) = delete;
        DescriptorSet &operator=(DescriptorSet &&other) = delete;

        bool Init(LogicalDevice& device, VkDescriptorPool pool, VkDescriptorSetLayout layout);
        void Destroy(LogicalDevice& device);
    };
}  // namespace Nebula::Vulkan
