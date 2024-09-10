#pragma once

#include <cstdint>

#include "GraphicsContext.hpp"

namespace Astrelis
{
    class UniformBuffer
    {
    public:
        UniformBuffer()                                = default;
        virtual ~UniformBuffer()                       = default;
        UniformBuffer(const UniformBuffer&)            = default;
        UniformBuffer& operator=(const UniformBuffer&) = default;
        UniformBuffer(UniformBuffer&&)                 = default;
        UniformBuffer& operator=(UniformBuffer&&)      = default;

        virtual bool Init(RefPtr<GraphicsContext>& context, uint32_t size)                                       = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context) const                                             = 0;
        virtual void SetData(RefPtr<GraphicsContext>& context, const void* data, uint32_t size, uint32_t offset) = 0;
    };
} // namespace Astrelis
