#include "FileTree.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "imgui.h"

#include <stack>
#include <utility>

namespace Pulsar
{
    // Todo make not recursive
    static void AddNodes(FileTree::Node& node)
    {
        auto files = node.File.ListFiles();
        node.Nodes.reserve(files.size());
        for (const auto& file : files)
        {
            auto& curNode = node.Nodes.emplace_back(file);
            if (file.IsDirectory())
            {
                AddNodes(curNode);
            }
        }
    }

    FileTree::FileTree(Nebula::File root) : m_Root(std::move(root)) { AddNodes(m_Root); }

    static void ImGuiTreeRecursive(FileTree::Node& node, Nebula::RawRef<FileTree::Node*>& selectedNode)
    {
        if (node.File.IsDirectory())
        {
            if (ImGui::TreeNode(node.File.GetFilename().c_str()))
            {
                for (auto& child : node.Nodes)
                {
                    ImGuiTreeRecursive(child, selectedNode);
                };
                ImGui::TreePop();
            }
        }
        else
        {
            bool selected = selectedNode != nullptr && *selectedNode == node;
            if (ImGui::Selectable(node.File.GetFilename().c_str(), selected))
            {
                selectedNode = Nebula::RawRef<FileTree::Node*>(&node);
            }
        };
    }

    Nebula::RawRef<FileTree::Node*> FileTree::ImGuiTree() {
        static Nebula::RawRef<FileTree::Node*> selected = nullptr;
        ImGuiTreeRecursive(m_Root, selected);
        return selected;
    }
} // namespace Pulsar
