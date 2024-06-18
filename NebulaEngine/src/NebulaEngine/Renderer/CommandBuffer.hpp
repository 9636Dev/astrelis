#pragma once

namespace Nebula
{
    class CommandBuffer
    {
    public:
        CommandBuffer() = default;
        virtual ~CommandBuffer() = default;
        CommandBuffer(const CommandBuffer&) = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;
        CommandBuffer(CommandBuffer&&) = delete;
        CommandBuffer& operator=(CommandBuffer&&) = delete;

        virtual bool Begin() { return true; }
        virtual bool End() { return true; }
    };
} // namespace Nebula
