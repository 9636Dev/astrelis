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
        TextureSampler(const TextureSampler&)            = default;
        TextureSampler& operator=(const TextureSampler&) = default;
        TextureSampler(TextureSampler&&)                 = default;
        TextureSampler& operator=(TextureSampler&&)      = default;

        [[nodiscard]] bool Init(LogicalDevice& device, PhysicalDevice& physicalDevice);
        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context) override;
        void               Destroy(LogicalDevice& device);
        void               Destroy(RefPtr<GraphicsContext>& context) override;

        VkSampler m_Sampler = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
