#include "IndexBuffer.hpp"

#include <cstring>

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "LogicalDevice.hpp"
#include "Utils.hpp"

#include "Astrelis/Core/Log.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Astrelis::Vulkan
{
    bool IndexBuffer::Init(LogicalDevice& device, PhysicalDevice& physicalDevice, std::uint32_t count)
    {
        VkDeviceSize bufferSize = sizeof(std::uint32_t) * count;
        if (!CreateBuffer(physicalDevice.GetHandle(), device.GetHandle(), bufferSize,
                          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Buffer,
                          m_BufferMemory))
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to create index buffer!");
            return false;
        }
        vkBindBufferMemory(device.GetHandle(), m_Buffer, m_BufferMemory, 0);

        return true;
    }

    bool IndexBuffer::Init(RefPtr<GraphicsContext>& context, std::uint32_t count)
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        return Init(ctx->m_LogicalDevice, ctx->m_PhysicalDevice, count);
    }

    void IndexBuffer::Destroy(RefPtr<GraphicsContext>& context)
    {
        auto& logicalDevice = context.As<VulkanGraphicsContext>()->m_LogicalDevice;
        vkDestroyBuffer(logicalDevice.GetHandle(), m_Buffer, nullptr);
        vkFreeMemory(logicalDevice.GetHandle(), m_BufferMemory, nullptr);
    }

    bool IndexBuffer::SetData(LogicalDevice& device,
                              PhysicalDevice& physicalDevice,
                              CommandPool& commandPool,
                              const std::uint32_t* data,
                              std::uint32_t count)
    {
        VkBuffer stagingBuffer             = VK_NULL_HANDLE;
        VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;
        auto size                          = sizeof(std::uint32_t) * count;
        if (!CreateBuffer(physicalDevice.GetHandle(), device.GetHandle(), size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                          stagingBufferMemory))
        {
            return false;
        }
        vkBindBufferMemory(device.GetHandle(), stagingBuffer, stagingBufferMemory, 0);

        void* mappedData = nullptr;
        vkMapMemory(device.GetHandle(), stagingBufferMemory, 0, size, 0, &mappedData);
        memcpy(mappedData, data, size);
        vkUnmapMemory(device.GetHandle(), stagingBufferMemory);

        if (!CopyBuffer(device.GetHandle(), device.GetGraphicsQueue(), commandPool.GetHandle(), stagingBuffer, m_Buffer,
                        size))
        {
            return false;
        }

        vkDestroyBuffer(device.GetHandle(), stagingBuffer, nullptr);
        vkFreeMemory(device.GetHandle(), stagingBufferMemory, nullptr);

        return true;
    }

    bool IndexBuffer::SetData(RefPtr<Astrelis::GraphicsContext>& context, const std::uint32_t* data, std::uint32_t count)
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        return SetData(ctx->m_LogicalDevice, ctx->m_PhysicalDevice, ctx->m_CommandPool, data, count);
    }

    void IndexBuffer::Bind(CommandBuffer& buffer) const
    {
        vkCmdBindIndexBuffer(buffer.GetHandle(), m_Buffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void IndexBuffer::Bind(RefPtr<GraphicsContext>& context) const
    {
        Bind(context.As<VulkanGraphicsContext>()->GetCurrentFrame().CommandBuffer);
    }
} // namespace Astrelis::Vulkan
