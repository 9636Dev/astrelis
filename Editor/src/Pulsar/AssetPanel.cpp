#include "AssetPanel.hpp"

#include <imgui.h>

#include "Astrelis/Core/Log.hpp"
#include "Astrelis/Core/Utils/Defer.hpp"

namespace Pulsar
{
    AssetPanel::AssetPanel(Astrelis::File rootDirectory) : m_FileTree(std::move(rootDirectory)) {}

    void AssetPanel::Draw()
    {
        ImGui::Begin("Assets");
        Astrelis::Deferred deferredImGuiEnd([]() { ImGui::End(); });

        ImGuiTableFlags tableFlags = ImGuiTableFlags_None;
        tableFlags |= ImGuiTableFlags_BordersInnerV;
        tableFlags |= ImGuiTableFlags_Resizable;
        tableFlags |= ImGuiTableFlags_SizingStretchSame;
        tableFlags |= ImGuiTableFlags_ScrollX;
        tableFlags |= ImGuiTableFlags_ScrollY;

        if (!ImGui::BeginTable("assets_table", 2, tableFlags, ImGui::GetWindowSize()))
        {
            return;
        }


        ImGui::TableNextColumn();

        // Directory tree
        Astrelis::RawRef<FileTree::Node*> selected = m_FileTree.ImGuiTree();

        ImGui::TableNextColumn();

        if (selected == nullptr)
        {
            ImGui::Text("Please select a file to view the contents");
        }
        else
        {
            static Astrelis::RawRef<FileTree::Node*> cachedNode = nullptr;
            static std::string cachedContents;

            if (cachedNode != selected)
            {
                cachedNode = selected;
                auto res = selected->File.ReadText();
                if (res.IsErr())
                {
                    cachedContents = "Failed to read file: " + res.UnwrapErr();
                }
                else
                {
                    cachedContents = res.Unwrap();
                }
            }

            ImGui::Text("%s", cachedContents.c_str());
        }

        ImGui::EndTable();
    }
} // namespace Pulsar
