#include "AssetPanel.hpp"

#include <imgui.h>

AssetPanel::AssetPanel(std::string rootDirectory)
    : m_RootDirectory(std::move(rootDirectory))
{
}

void AssetPanel::Draw()
{
    ImGui::Begin("Assets");
    ImGui::Text("Root Directory: %s", m_RootDirectory.c_str());
    ImGui::End();
}
