#pragma once

#include "NebulaEngine/Renderer/UniformBuffer.hpp"

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"

#include <cstdint>
#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class UniformBuffer : public Nebula::UniformBuffer
    {
    public:
        UniformBuffer() = default;
        ~UniformBuffer() override = default;
        UniformBuffer(const UniformBuffer &) = delete;
        UniformBuffer &operator=(const UniformBuffer &) = delete;
        UniformBuffer(UniformBuffer &&) = delete;
        UniformBuffer &operator=(UniformBuffer &&) = delete;

        bool Init(PhysicalDevice& physicalDevice, LogicalDevice &device, std::uint32_t size);
        void Destroy(LogicalDevice &device) const;

        void SetData(const void *data, std::uint32_t size, std::uint32_t offset) override;

        VkBuffer m_Buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
        void *m_MappedMemory = nullptr;
    };
} // namespace Nebula::Vulkan
