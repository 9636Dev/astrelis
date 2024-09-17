#include "AssetPanel.hpp"

#include "Astrelis/Core/Log.hpp"

#include <imgui.h>

namespace AstrelisEditor {
    AssetPanel::AssetPanel(Astrelis::File rootDirectory) : m_FileTree(std::move(rootDirectory)) {
    }

    void AssetPanel::Draw() {
        ImGui::Begin("Assets");

        Astrelis::RawRef<FileTree::Node*> selected = m_FileTree.ImGuiTree();
        if (selected != nullptr) {
            ASTRELIS_LOG_INFO("Selected: {0}", selected->File.GetFilename());
        }


        ImGui::End();
    }
} // namespace AstrelisEditor
