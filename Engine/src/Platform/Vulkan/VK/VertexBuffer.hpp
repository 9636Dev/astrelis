#pragma once

#include "Astrelis/Renderer/VertexBuffer.hpp"

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"

//#include <vulkan/vulkan.h>

typedef struct VkBuffer_T*       VkBuffer;
typedef struct VkDeviceMemory_T* VkDeviceMemory;

namespace Astrelis::Vulkan {
    class VertexBuffer : public Astrelis::VertexBuffer {
    public:
        VertexBuffer()                               = default;
        ~VertexBuffer() override                     = default;
        VertexBuffer(const VertexBuffer&)            = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&)                 = delete;
        VertexBuffer& operator=(VertexBuffer&&)      = delete;

        [[nodiscard]] bool Init(
            LogicalDevice& device, PhysicalDevice& physicalDevice, std::size_t size);
        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context, std::size_t size) override;
        void               Destroy(RefPtr<GraphicsContext>& context) override;

        [[nodiscard]] bool SetData(LogicalDevice& device, PhysicalDevice& physicalDevice,
            CommandPool& commandPool, const void* data, std::size_t size);
        [[nodiscard]] bool SetData(
            RefPtr<GraphicsContext>& context, const void* data, std::size_t size) override;
        void Bind(CommandBuffer& buffer, std::uint32_t binding) const;
        void Bind(RefPtr<GraphicsContext>& context, std::uint32_t binding) const override;
    private:
        VkBuffer       m_Buffer       = nullptr;
        VkDeviceMemory m_BufferMemory = nullptr;
    };
} // namespace Astrelis::Vulkan
