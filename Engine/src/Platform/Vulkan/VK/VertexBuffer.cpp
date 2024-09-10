#include "VertexBuffer.hpp"

#include <array>
#include <cstring>

#include <vulkan/vulkan.h>

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "Utils.hpp"

#include "Astrelis/Core/Log.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Astrelis::Vulkan
{
    bool VertexBuffer::Init(LogicalDevice& device, PhysicalDevice& physicalDevice, std::size_t size)
    {
        VkDeviceSize bufferSize = size;
        if (!CreateBuffer(physicalDevice.GetHandle(), device.GetHandle(), bufferSize,
                          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Buffer,
                          m_BufferMemory))
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to create vertex buffer!");
            return false;
        }
        vkBindBufferMemory(device.GetHandle(), m_Buffer, m_BufferMemory, 0);

        return true;
    }

    bool VertexBuffer::Init(RefPtr<GraphicsContext>& context, std::size_t size)
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        return Init(ctx->m_LogicalDevice, ctx->m_PhysicalDevice, size);
    }

    void VertexBuffer::Destroy(RefPtr<GraphicsContext>& context)
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        vkDestroyBuffer(ctx->m_LogicalDevice.GetHandle(), m_Buffer, nullptr);
        vkFreeMemory(ctx->m_LogicalDevice.GetHandle(), m_BufferMemory, nullptr);
    }

    bool VertexBuffer::SetData(LogicalDevice& device,
                               PhysicalDevice& physicalDevice,
                               CommandPool& commandPool,
                               const void* data,
                               std::size_t size)
    {
        VkBuffer stagingBuffer             = VK_NULL_HANDLE;
        VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;
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

    bool VertexBuffer::SetData(RefPtr<Astrelis::GraphicsContext>& context, const void* data, std::size_t size)
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        return SetData(ctx->m_LogicalDevice, ctx->m_PhysicalDevice, ctx->m_CommandPool, data, size);
    }

    void VertexBuffer::Bind(CommandBuffer& buffer, std::uint32_t binding) const
    {
        std::array<VkBuffer, 1> buffers     = {m_Buffer};
        std::array<VkDeviceSize, 1> offsets = {0};
        vkCmdBindVertexBuffers(buffer.GetHandle(), binding, buffers.size(), buffers.data(), offsets.data());
    }

    void VertexBuffer::Bind(RefPtr<GraphicsContext>& context, std::uint32_t binding) const
    {
        Bind(context.As<VulkanGraphicsContext>()->GetCurrentFrame().CommandBuffer, binding);
    }
} // namespace Astrelis::Vulkan
