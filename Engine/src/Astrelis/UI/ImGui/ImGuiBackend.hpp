#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Window.hpp"

namespace Astrelis
{
    class ImGuiBackend
    {
    public:
        ImGuiBackend()                               = default;
        virtual ~ImGuiBackend()                      = default;
        ImGuiBackend(const ImGuiBackend&)            = delete;
        ImGuiBackend& operator=(const ImGuiBackend&) = delete;
        ImGuiBackend(ImGuiBackend&&)                 = delete;
        ImGuiBackend& operator=(ImGuiBackend&&)      = delete;

        virtual void Init()                        = 0;
        virtual void Shutdown()                    = 0;
        virtual void Begin()                       = 0;
        virtual void End()                         = 0;
        virtual void Resize(int width, int height) = 0;

        static RefPtr<ImGuiBackend> Create(RefPtr<Window> window);
    };
} // namespace Astrelis
