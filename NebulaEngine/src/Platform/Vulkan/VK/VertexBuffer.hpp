#pragma once

#include "NebulaEngine/Renderer/VertexBuffer.hpp"

//#include <vulkan/vulkan.h>

typedef struct VkBuffer_T* VkBuffer;
typedef struct VkDeviceMemory_T* VkDeviceMemory;

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

        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context, std::size_t size) override;
        void Destroy(RefPtr<GraphicsContext>& context) override;

        [[nodiscard]] bool SetData(RefPtr<GraphicsContext>& context, const void* data, std::size_t size) override;
        void Bind(RefPtr<GraphicsContext>& context, std::uint32_t binding) const override;

    private:
        VkBuffer m_Buffer = nullptr;
        VkDeviceMemory m_BufferMemory = nullptr;
    };
} // namespace Nebula::Vulkan
