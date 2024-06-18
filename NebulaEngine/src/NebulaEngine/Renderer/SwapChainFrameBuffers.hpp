#pragma once

namespace Nebula
{
    class SwapChainFrameBuffers
    {
    public:
        SwapChainFrameBuffers() = default;
        virtual ~SwapChainFrameBuffers() = default;
        SwapChainFrameBuffers(const SwapChainFrameBuffers &) = delete;
        SwapChainFrameBuffers &operator=(const SwapChainFrameBuffers &) = delete;
        SwapChainFrameBuffers(SwapChainFrameBuffers &&) = delete;
        SwapChainFrameBuffers &operator=(SwapChainFrameBuffers &&) = delete;
    };
} // namespace Nebula
