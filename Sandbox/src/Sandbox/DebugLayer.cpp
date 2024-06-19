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
    ImGui::Begin("Debug Layer");

    ImGui::Text("Hello, World!");

    ImGui::End();
}

void DebugLayer::OnEvent(Nebula::Event& event)
{
    (void)event;
}
