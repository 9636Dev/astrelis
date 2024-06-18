#pragma once

namespace Nebula
{
    class Semaphore
    {
    public:
        Semaphore() = default;
        virtual ~Semaphore() = default;
        Semaphore(const Semaphore &) = delete;
        Semaphore &operator=(const Semaphore &) = delete;
        Semaphore(Semaphore &&) = delete;
        Semaphore &operator=(Semaphore &&) = delete;
    };
} // namespace Nebula
