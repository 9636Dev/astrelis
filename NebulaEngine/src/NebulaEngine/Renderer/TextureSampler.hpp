#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "GraphicsContext.hpp"

namespace Nebula
{
    class TextureSampler
    {
    public:
        TextureSampler() = default;
        virtual ~TextureSampler() = default;
        TextureSampler(const TextureSampler&) = delete;
        TextureSampler& operator=(const TextureSampler&) = delete;
        TextureSampler(TextureSampler&&) = delete;
        TextureSampler& operator=(TextureSampler&&) = delete;

        virtual bool Init(RefPtr<GraphicsContext>& context) = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context) = 0;
    };
} // namespace Nebula
