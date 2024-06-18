#pragma once

namespace Nebula
{
    /**
    * @note Not pure virtual, because some implementations may not need to override this (e.g. OpenGL has no CommandPool)
    */
    class CommandPool
    {
    public:
        CommandPool() = default;
        virtual ~CommandPool() = default;
        CommandPool(const CommandPool&) = delete;
        CommandPool& operator=(const CommandPool&) = delete;
        CommandPool(CommandPool&&) = delete;
        CommandPool& operator=(CommandPool&&) = delete;
    };
} // namespace Nebula
