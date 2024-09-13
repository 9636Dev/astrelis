#pragma once

#include "Astrelis/Core/Layer.hpp"
#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Events/WindowEvent.hpp"

#include <imgui.h>

#include "ImGuiBackend.hpp"

namespace Astrelis
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
} // namespace Astrelis
