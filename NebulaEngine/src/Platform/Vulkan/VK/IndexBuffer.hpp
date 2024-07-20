#pragma once

#include <vulkan/vulkan.h>

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"

#include "NebulaEngine/Renderer/IndexBuffer.hpp"

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

        [[nodiscard]] bool Init(LogicalDevice& device, PhysicalDevice& physicalDevice, std::uint32_t count);
        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context, std::uint32_t count) override;
        void Destroy(RefPtr<GraphicsContext>& context) override;

        [[nodiscard]] bool SetData(LogicalDevice& device,
                                   PhysicalDevice& physicalDevice,
                                   CommandPool& commandPool,
                                   const std::uint32_t* data,
                                   std::uint32_t count);
        [[nodiscard]] bool
            SetData(RefPtr<GraphicsContext>& context, const std::uint32_t* data, std::uint32_t count) override;
        void Bind(CommandBuffer& buffer) const;
        void Bind(RefPtr<Nebula::GraphicsContext>& context) const override;
    private:
        VkBuffer m_Buffer             = VK_NULL_HANDLE;
        VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
