#pragma once

#include "NebulaEngine/Renderer/ImageViews.hpp"
#include "Platform/Vulkan/VK/LogicalDevice.hpp"
#include "Platform/Vulkan/VK/SwapChain.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class ImageViews : public Nebula::ImageViews
    {
    public:
        ImageViews()                             = default;
        ~ImageViews() override                   = default;
        ImageViews(const ImageViews&)            = delete;
        ImageViews& operator=(const ImageViews&) = delete;
        ImageViews(ImageViews&&)                 = delete;
        ImageViews& operator=(ImageViews&&)      = delete;

        bool Init(LogicalDevice& device, SwapChain& swapChain);
        void Destroy(LogicalDevice& device);

        [[nodiscard]] const std::vector<VkImageView>& GetImageViews() const { return m_ImageViews; }
    private:
        std::vector<VkImageView> m_ImageViews;
    };
} // namespace Nebula::Vulkan
