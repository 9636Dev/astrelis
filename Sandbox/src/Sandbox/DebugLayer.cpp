#include "DebugLayer.hpp"

#include "NebulaEngine/Core/Log.hpp"

#include <imgui.h>

void DebugLayer::OnAttach()
{
}

void DebugLayer::OnDetach()
{
}

void DebugLayer::OnUIRender()
{
    ImGui::Begin("Debug Info");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}

void DebugLayer::OnEvent(Nebula::Event& event)
{
    (void)event;
}
