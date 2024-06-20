#include "UniformBuffer.hpp"

#include <cstring>

#include "NebulaEngine/Core/Log.hpp"

#include "Utils.hpp"

namespace Nebula::Vulkan
{
    bool UniformBuffer::Init(PhysicalDevice& physicalDevice, LogicalDevice& device, std::uint32_t size)
    {
        VkDeviceSize bufferSize = size;

        if (!CreateBuffer(
                physicalDevice.GetHandle(), device.GetHandle(), bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Buffer, m_Memory))
        {
            return false;
        }

        if (vkMapMemory(device.GetHandle(), m_Memory, 0, bufferSize, 0, &m_MappedMemory) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to map uniform buffer memory!");
            return false;
        }
        return true;
    }

    void UniformBuffer::Destroy(LogicalDevice& device) const
    {
        vkUnmapMemory(device.GetHandle(), m_Memory);
        vkDestroyBuffer(device.GetHandle(), m_Buffer, nullptr);
        vkFreeMemory(device.GetHandle(), m_Memory, nullptr);
    }

    void UniformBuffer::SetData(const void* data, std::uint32_t size, std::uint32_t offset)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        std::memcpy(static_cast<std::uint8_t*>(m_MappedMemory) + offset, data, size);
    }
} // namespace Nebula::Vulkan
