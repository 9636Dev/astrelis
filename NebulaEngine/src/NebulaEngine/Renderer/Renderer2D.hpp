#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Math.hpp"

namespace Nebula
{
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene();
        static void EndScene();
        static void SetViewport(const Bounds& bounds);

        static void DrawQuad(const Vector2f& position, const Vector2f& size, const Vector4f& color);

    private:
        static void Flush();
        static void StartBatch();
        static void EndBatch();
        static void NextBatch();
    };
} // namespace Nebula
