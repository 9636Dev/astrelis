#include "UniformBuffer.hpp"

#include "Astrelis/Core/Log.hpp"

#include <cstring>

#include "Platform/Vulkan/VulkanGraphicsContext.hpp"
#include "Utils.hpp"

namespace Astrelis::Vulkan {
    bool UniformBuffer::Init(RefPtr<GraphicsContext>& context, std::uint32_t size) {
        auto         ctx        = context.As<VulkanGraphicsContext>();
        VkDeviceSize bufferSize = size;

        m_Buffers.resize(ctx->m_Frames.size());
        for (Buffer& buffer : m_Buffers) {
            if (!CreateBuffer(ctx->m_PhysicalDevice.GetHandle(), ctx->m_LogicalDevice.GetHandle(),
                    bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    buffer.m_Buffer, buffer.m_Memory)) {
                return false;
            }

            vkBindBufferMemory(
                ctx->m_LogicalDevice.GetHandle(), buffer.m_Buffer, buffer.m_Memory, 0);
            if (vkMapMemory(ctx->m_LogicalDevice.GetHandle(), buffer.m_Memory, 0, bufferSize, 0,
                    &buffer.m_MappedMemory)
                != VK_SUCCESS) {
                ASTRELIS_CORE_LOG_ERROR("Failed to map uniform buffer memory!");
                return false;
            }
        }
        return true;
    }

    void UniformBuffer::Destroy(RefPtr<GraphicsContext>& context) const {
        auto& device = context.As<VulkanGraphicsContext>()->m_LogicalDevice;
        for (const Buffer& buffer : m_Buffers) {
            vkUnmapMemory(device.GetHandle(), buffer.m_Memory);
            vkDestroyBuffer(device.GetHandle(), buffer.m_Buffer, nullptr);
            vkFreeMemory(device.GetHandle(), buffer.m_Memory, nullptr);
        }
    }

    void UniformBuffer::SetData(RefPtr<GraphicsContext>& context, const void* data,
        std::uint32_t size, std::uint32_t offset) {
        (void)context;
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        std::memcpy(static_cast<std::uint8_t*>(m_Buffers[0].m_MappedMemory) + offset, data, size);
    }
} // namespace Astrelis::Vulkan
