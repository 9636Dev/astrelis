#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/IO/File.hpp"

#include <utility>
#include <vector>

namespace Pulsar
{
    class FileTree
    {
    public:
        struct Node
        {
            Nebula::File File;
            std::vector<Node> Nodes;

            explicit Node(Nebula::File file) : File(std::move(file)) {}

            bool operator==(const Node& other) const
            {
                return File == other.File;
            }
        };

        explicit FileTree(Nebula::File root);

        Nebula::RawRef<Node*> ImGuiTree();

    private:
        Node m_Root;
    };
} // namespace Pulsar
