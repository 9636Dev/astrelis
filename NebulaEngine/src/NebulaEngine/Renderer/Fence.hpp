#pragma once

#include "GraphicsContext.hpp"

#include <cstdint>

namespace Nebula
{
    class Fence
    {
    public:
        Fence()                        = default;
        virtual ~Fence()               = default;
        Fence(const Fence&)            = delete;
        Fence& operator=(const Fence&) = delete;
        Fence(Fence&&)                 = delete;
        Fence& operator=(Fence&&)      = delete;

        virtual void Wait(RefPtr<GraphicsContext>& context, std::uint64_t timeout) = 0;
        virtual void Reset(RefPtr<GraphicsContext>& context) = 0;
    };
} // namespace Nebula
