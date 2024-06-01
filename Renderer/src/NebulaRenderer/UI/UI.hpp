#pragma once

#include "../Windowing/Window.hpp"

namespace Nebula
{
    class UI
    {
    public:
        UI() = delete;

        static void Init(const Ref<Window>& window);
        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();
    };
} // namespace Nebula
