#pragma once

#include <imgui.h>

#include "ImGuiBackend.hpp"

#include "NebulaEngine/Core/Layer.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Events/WindowEvent.hpp"

namespace Nebula
{
    class ImGuiLayer : public Layer
    {
    public:
        explicit ImGuiLayer(RefPtr<ImGuiBackend> backend);
        ~ImGuiLayer() override                   = default;
        ImGuiLayer(const ImGuiLayer&)            = delete;
        ImGuiLayer& operator=(const ImGuiLayer&) = delete;
        ImGuiLayer(ImGuiLayer&&)                 = delete;
        ImGuiLayer& operator=(ImGuiLayer&&)      = delete;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;

        void Begin();
        void End();

        void SetDarkThemeColors();

        void BlockEvents(bool block) { m_BlockEvents = block; }
    private:
        void Resize(ViewportResizedEvent& event);

        bool m_BlockEvents = true;
        RefPtr<ImGuiBackend> m_Backend;
    };
} // namespace Nebula
