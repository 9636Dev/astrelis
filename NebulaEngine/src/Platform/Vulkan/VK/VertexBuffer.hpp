#pragma once

#include "NebulaEngine/Renderer/CommandPool.hpp"
#include "NebulaEngine/Renderer/VertexBuffer.hpp"
#include "Platform/Vulkan/VK/LogicalDevice.hpp"
#include "Platform/Vulkan/VK/PhysicalDevice.hpp"

namespace Nebula::Vulkan
{
    class VertexBuffer : public Nebula::VertexBuffer
    {
    public:
        VertexBuffer() = default;
        ~VertexBuffer() override = default;
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer& operator=(VertexBuffer&&) = delete;

        bool Init(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, std::size_t size);
        void Destroy(LogicalDevice& logicalDevice);

        bool SetData(RefPtr<GraphicsContext>& context, RefPtr<Nebula::CommandPool>& commandPool, const void* data, std::size_t size) override;
        void Bind(RefPtr<Nebula::CommandBuffer>& buffer) const override;

    private:
        VkBuffer m_Buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
