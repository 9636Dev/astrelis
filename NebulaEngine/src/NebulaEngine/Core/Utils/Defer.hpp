#pragma once

#include <functional>
#include <utility>

namespace Nebula
{
    class Deferred
    {
    public:
        explicit Deferred(std::function<void()> function) : m_Function(std::move(function)) {}

        ~Deferred()
        {
            if (m_Function != nullptr)
            {
                m_Function();
            }
        }

        Deferred(const Deferred&)            = default;
        Deferred(Deferred&&)                 = default;
        Deferred& operator=(const Deferred&) = default;
        Deferred& operator=(Deferred&&)      = default;

        void Reset() { m_Function = nullptr; }
    private:
        std::function<void()> m_Function;
    };
} // namespace Nebula
