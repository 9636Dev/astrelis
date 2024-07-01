#pragma once

#include "GraphicsContext.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

namespace Nebula
{
    class TextureSampler
    {
    public:
        TextureSampler()                                 = default;
        virtual ~TextureSampler()                        = default;
        TextureSampler(const TextureSampler&)            = default;
        TextureSampler& operator=(const TextureSampler&) = default;
        TextureSampler(TextureSampler&&)                 = default;
        TextureSampler& operator=(TextureSampler&&)      = default;

        virtual bool Init(RefPtr<GraphicsContext>& context)    = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context) = 0;
    };
} // namespace Nebula
