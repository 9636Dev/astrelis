#pragma once

#include "Fence.hpp"
#include "GraphicsContext.hpp"
#include "Semaphore.hpp"

namespace Nebula
{
    class CommandBuffer
    {
    public:
        CommandBuffer()                                = default;
        virtual ~CommandBuffer()                       = default;
        CommandBuffer(const CommandBuffer&)            = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;
        CommandBuffer(CommandBuffer&&)                 = delete;
        CommandBuffer& operator=(CommandBuffer&&)      = delete;

        virtual bool Begin() { return true; }

        virtual bool End() { return true; }

        virtual void Reset() {}

        virtual bool Submit([[maybe_unused]] RefPtr<GraphicsContext>& context,
                            [[maybe_unused]] RefPtr<Semaphore>& imageAvailableSemaphore,
                            [[maybe_unused]] RefPtr<Semaphore>& finishedRenderingSemaphore,
                            [[maybe_unused]] RefPtr<Fence>& inFlightFence)
        {
            return true;
        }
    };
} // namespace Nebula
