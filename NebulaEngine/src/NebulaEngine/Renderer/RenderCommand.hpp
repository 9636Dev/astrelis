#pragma once

#include "NebulaEngine/Core/Math.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"

namespace Nebula
{
    class RenderCommand
    {
public:
        static void Init();
        static void SetViewport(std::int32_t posX, std::int32_t posY, std::int32_t width, std::int32_t height);
        static void SetClearColor(const Vector4f &color);
        static void Clear();
private:
        static Ptr<RendererAPI> s_RendererAPI;
    };
} // namespace Nebula
