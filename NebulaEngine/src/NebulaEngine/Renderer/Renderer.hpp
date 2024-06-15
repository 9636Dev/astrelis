#pragma once

#include "RendererAPI.hpp"

namespace Nebula
{
    class Renderer
    {
    public:
        static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };
} // namespace Nebula
