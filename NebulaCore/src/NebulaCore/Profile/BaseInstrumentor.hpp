#pragma once

#include <memory>

#include "BasicJson.hpp"

namespace Nebula::Profiling
{
    class BaseInstrumentor
    {
    public:
        class Scoped
        {
        public:
            Scoped() = default;
            Scoped(const Scoped&)            = delete;
            Scoped(Scoped&&)                 = delete;
            Scoped& operator=(const Scoped&) = delete;
            Scoped& operator=(Scoped&&)      = delete;
            virtual ~Scoped()                 = default;

            virtual void End(JsonObject& json) = 0;
        };

        BaseInstrumentor()                                   = default;
        BaseInstrumentor(const BaseInstrumentor&)            = delete;
        BaseInstrumentor(BaseInstrumentor&&)                 = delete;
        BaseInstrumentor& operator=(const BaseInstrumentor&) = delete;
        BaseInstrumentor& operator=(BaseInstrumentor&&)      = delete;
        virtual ~BaseInstrumentor()                          = default;

        virtual std::unique_ptr<Scoped> Scope() = 0;
    };
} // namespace Nebula::Profiling
