#pragma once

#include "Astrelis/Renderer/TextureSampler.hpp"

#include <vulkan/vulkan.h>

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"

namespace Astrelis::Vulkan {
    class TextureSampler : public Astrelis::TextureSampler {
    public:
        TextureSampler()                                 = default;
        ~TextureSampler() override                       = default;
        TextureSampler(const TextureSampler&)            = delete;
        TextureSampler& operator=(const TextureSampler&) = delete;
        TextureSampler(TextureSampler&&)                 = delete;
        TextureSampler& operator=(TextureSampler&&)      = delete;

        bool Init(LogicalDevice& device, PhysicalDevice& physicalDevice);
        bool Init(RefPtr<GraphicsContext>& context) override;
        void Destroy(RefPtr<GraphicsContext>& context) override;

        VkSampler m_Sampler = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
