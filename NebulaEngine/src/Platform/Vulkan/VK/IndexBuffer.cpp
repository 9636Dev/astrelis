#include "IndexBuffer.hpp"

#include <cstring>

#include "NebulaEngine/Core/Log.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "Utils.hpp"

namespace Nebula::Vulkan
{
    bool IndexBuffer::Init(RefPtr<GraphicsContext>& context, std::uint32_t count)
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        VkDeviceSize bufferSize = sizeof(std::uint32_t) * count;
        if (!CreateBuffer(ctx->m_PhysicalDevice.GetHandle(), ctx->m_LogicalDevice.GetHandle(), bufferSize,
                          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Buffer,
                          m_BufferMemory))
        {
            NEBULA_CORE_LOG_ERROR("Failed to create index buffer!");
            return false;
        }
        vkBindBufferMemory(ctx->m_LogicalDevice.GetHandle(), m_Buffer, m_BufferMemory, 0);

        return true;
    }

    void IndexBuffer::Destroy(RefPtr<GraphicsContext>& context)
    {
        auto& logicalDevice = context.As<VulkanGraphicsContext>()->m_LogicalDevice;
        vkDestroyBuffer(logicalDevice.GetHandle(), m_Buffer, nullptr);
        vkFreeMemory(logicalDevice.GetHandle(), m_BufferMemory, nullptr);
    }

    bool IndexBuffer::SetData(RefPtr<Nebula::GraphicsContext>& context, const std::uint32_t* data, std::uint32_t count)
    {
        auto ctx                           = context.As<VulkanGraphicsContext>();
        VkBuffer stagingBuffer             = VK_NULL_HANDLE;
        VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;
        auto size                          = sizeof(std::uint32_t) * count;
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

    void IndexBuffer::Bind(RefPtr<GraphicsContext>& context) const
    {
        vkCmdBindIndexBuffer(context.As<VulkanGraphicsContext>()->GetCurrentFrame().CommandBuffer.GetHandle(), m_Buffer,
                             0, VK_INDEX_TYPE_UINT32);
    }
} // namespace Nebula::Vulkan
