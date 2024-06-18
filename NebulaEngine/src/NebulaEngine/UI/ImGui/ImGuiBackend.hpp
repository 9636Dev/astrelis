#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Window.hpp"

namespace Nebula
{
    class ImGuiBackend
    {
    public:
        ImGuiBackend() = default;
        virtual ~ImGuiBackend() = default;
        ImGuiBackend(const ImGuiBackend&) = delete;
        ImGuiBackend& operator=(const ImGuiBackend&) = delete;
        ImGuiBackend(ImGuiBackend&&) = delete;
        ImGuiBackend& operator=(ImGuiBackend&&) = delete;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;
        virtual void Begin() = 0;
        virtual void End() = 0;

        static RefPtr<ImGuiBackend> Create(RefPtr<Window> window);
    };
} // namespace Nebula
