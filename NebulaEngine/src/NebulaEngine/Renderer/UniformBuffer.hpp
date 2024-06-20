#pragma once

#include <cstdint>

namespace Nebula
{
    class UniformBuffer
    {
    public:
        UniformBuffer() = default;
        virtual ~UniformBuffer() = default;
        UniformBuffer(const UniformBuffer &) = delete;
        UniformBuffer &operator=(const UniformBuffer &) = delete;
        UniformBuffer(UniformBuffer &&) = delete;
        UniformBuffer &operator=(UniformBuffer &&) = delete;

        virtual void SetData(const void *data, uint32_t size, uint32_t offset) = 0;
    };
}  // namespace Nebula
