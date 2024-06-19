#pragma once

#include "NebulaEngine/Renderer/IndexBuffer.hpp"

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class IndexBuffer : public Nebula::IndexBuffer
    {
    public:
        IndexBuffer()                              = default;
        ~IndexBuffer() override                    = default;
        IndexBuffer(const IndexBuffer&)            = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&)                 = delete;
        IndexBuffer& operator=(IndexBuffer&&)      = delete;

        bool Init(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, std::uint32_t count);
        void Destroy(LogicalDevice& logicalDevice);

        bool SetData(RefPtr<GraphicsContext>& context,
                     RefPtr<Nebula::CommandPool>& commandPool,
                     const std::uint32_t* data,
                     std::uint32_t count) override;
        void Bind(RefPtr<Nebula::CommandBuffer>& buffer) const override;
    private:
        VkBuffer m_Buffer             = VK_NULL_HANDLE;
        VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
