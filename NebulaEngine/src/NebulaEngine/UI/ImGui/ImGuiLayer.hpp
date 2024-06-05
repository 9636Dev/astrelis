#pragma once

#include "NebulaEngine/Core/Layer.hpp"

#include <imgui.h>

namespace Nebula
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;
        ImGuiLayer(const ImGuiLayer&) = delete;
        ImGuiLayer& operator=(const ImGuiLayer&) = delete;
        ImGuiLayer(ImGuiLayer&&) = delete;
        ImGuiLayer& operator=(ImGuiLayer&&) = delete;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;


        void Begin();
        void End();

        void SetDarkThemeColors();
        void BlockEvents(bool block) { m_BlockEvents = block; }
    private:
        bool m_BlockEvents = true;
    };
} // namespace Nebula
