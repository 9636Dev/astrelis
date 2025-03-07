#include "FileTree.hpp"

#include "Astrelis/Core/Pointer.hpp"

#include <stack>
#include <utility>

#include "imgui.h"

namespace AstrelisEditor {
    // Todo make not recursive
    static void AddNodes(FileTree::Node& node) {
        auto files = node.File.ListFiles();
        node.Nodes.reserve(files.size());
        for (const auto& file : files) {
            auto& curNode = node.Nodes.emplace_back(file);
            if (file.IsDirectory()) {
                AddNodes(curNode);
            }
        }
    }

    FileTree::FileTree(Astrelis::File root) : m_Root(std::move(root)) {
        AddNodes(m_Root);
    }

    static void ImGuiTreeRecursive(
        FileTree::Node& node, Astrelis::RawRef<FileTree::Node*>& selectedNode) {
        if (node.File.IsDirectory()) {
            if (ImGui::TreeNode(node.File.GetFilename().c_str())) {
                for (auto& child : node.Nodes) {
                    ImGuiTreeRecursive(child, selectedNode);
                };
                ImGui::TreePop();
            }
        }
        else {
            bool selected = selectedNode != nullptr && *selectedNode == node;
            if (ImGui::Selectable(node.File.GetFilename().c_str(), selected)) {
                selectedNode = Astrelis::RawRef<FileTree::Node*>(&node);
            }
        };
    }

    Astrelis::RawRef<FileTree::Node*> FileTree::ImGuiTree() {
        static Astrelis::RawRef<FileTree::Node*> selected = nullptr;
        ImGuiTreeRecursive(m_Root, selected);
        return selected;
    }
} // namespace AstrelisEditor
