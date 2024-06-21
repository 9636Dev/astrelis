#pragma once

#include "NebulaEngine/Renderer/UniformBuffer.hpp"

#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class UniformBuffer : public Nebula::UniformBuffer
    {
    public:
        UniformBuffer()                                = default;
        ~UniformBuffer() override                      = default;
        UniformBuffer(const UniformBuffer&)            = delete;
        UniformBuffer& operator=(const UniformBuffer&) = delete;
        UniformBuffer(UniformBuffer&&)                 = delete;
        UniformBuffer& operator=(UniformBuffer&&)      = delete;

        [[nodiscard]] bool Init(RefPtr<GraphicsContext>& context, std::uint32_t size) override;
        void Destroy(RefPtr<GraphicsContext>& context) const override;

        void SetData(RefPtr<GraphicsContext>& context, const void* data, std::uint32_t size, std::uint32_t offset) override;

        struct Buffer
        {
            VkBuffer m_Buffer       = VK_NULL_HANDLE;
            VkDeviceMemory m_Memory = VK_NULL_HANDLE;
            void* m_MappedMemory    = nullptr;
        };
        std::vector<Buffer> m_Buffers;
    };
} // namespace Nebula::Vulkan
