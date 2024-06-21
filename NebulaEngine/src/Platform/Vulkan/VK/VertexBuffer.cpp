#include "VertexBuffer.hpp"

#include <cstring>

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "Utils.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{

    bool VertexBuffer::Init(RefPtr<GraphicsContext>& context, std::size_t size)
    {
        auto ctx                = context.As<VulkanGraphicsContext>();
        VkDeviceSize bufferSize = size;
        if (!CreateBuffer(ctx->m_PhysicalDevice.GetHandle(), ctx->m_LogicalDevice.GetHandle(), bufferSize,
                          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Buffer,
                          m_BufferMemory))
        {
            NEBULA_CORE_LOG_ERROR("Failed to create vertex buffer!");
            return false;
        }
        vkBindBufferMemory(ctx->m_LogicalDevice.GetHandle(), m_Buffer, m_BufferMemory, 0);

        return true;
    }

    void VertexBuffer::Destroy(RefPtr<GraphicsContext>& context)
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        vkDestroyBuffer(ctx->m_LogicalDevice.GetHandle(), m_Buffer, nullptr);
        vkFreeMemory(ctx->m_LogicalDevice.GetHandle(), m_BufferMemory, nullptr);
    }

    bool VertexBuffer::SetData(RefPtr<Nebula::GraphicsContext>& context, const void* data, std::size_t size)
    {
        auto ctx                           = context.As<VulkanGraphicsContext>();
        VkBuffer stagingBuffer             = VK_NULL_HANDLE;
        VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;
        if (!CreateBuffer(ctx->m_PhysicalDevice.GetHandle(), ctx->m_LogicalDevice.GetHandle(), size,
                          VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                          stagingBufferMemory))
        {
            return false;
        }
        vkBindBufferMemory(ctx->m_LogicalDevice.GetHandle(), stagingBuffer, stagingBufferMemory, 0);

        void* mappedData = nullptr;
        vkMapMemory(ctx->m_LogicalDevice.GetHandle(), stagingBufferMemory, 0, size, 0, &mappedData);
        memcpy(mappedData, data, size);
        vkUnmapMemory(ctx->m_LogicalDevice.GetHandle(), stagingBufferMemory);

        if (!CopyBuffer(ctx->m_LogicalDevice.GetHandle(), ctx->m_LogicalDevice.GetGraphicsQueue(),
                        ctx->m_CommandPool.GetHandle(), stagingBuffer, m_Buffer, size))
        {
            return false;
        }

        vkDestroyBuffer(ctx->m_LogicalDevice.GetHandle(), stagingBuffer, nullptr);
        vkFreeMemory(ctx->m_LogicalDevice.GetHandle(), stagingBufferMemory, nullptr);

        return true;
    }

    void VertexBuffer::Bind(RefPtr<GraphicsContext>& context, std::uint32_t binding) const
    {
        std::array<VkBuffer, 1> buffers     = {m_Buffer};
        std::array<VkDeviceSize, 1> offsets = {0};
        vkCmdBindVertexBuffers(context.As<VulkanGraphicsContext>()->GetCurrentFrame().CommandBuffer.GetHandle(), binding,
                               buffers.size(), buffers.data(), offsets.data());
    }
} // namespace Nebula::Vulkan
