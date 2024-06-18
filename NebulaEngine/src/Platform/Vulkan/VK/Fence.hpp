#pragma once

#include "NebulaEngine/Renderer/Fence.hpp"
#include "LogicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class Fence : public Nebula::Fence
    {
    public:
        Fence() = default;
        ~Fence() override = default;
        Fence(const Fence&) = delete;
        Fence& operator=(const Fence&) = delete;
        Fence(Fence&&) = delete;
        Fence& operator=(Fence&&) = delete;

        bool Init(LogicalDevice& device);
        void Destroy(LogicalDevice& device);
        void Wait(RefPtr<GraphicsContext>& context, std::uint64_t timeout) override;
        void Reset(RefPtr<GraphicsContext>& context) override;

        VkFence GetHandle() const { return m_Fence; }

        VkFence m_Fence = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
