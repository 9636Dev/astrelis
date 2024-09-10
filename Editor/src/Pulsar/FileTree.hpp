#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/IO/File.hpp"

#include <utility>
#include <vector>

namespace Pulsar
{
    class FileTree
    {
    public:
        struct Node
        {
            Astrelis::File File;
            std::vector<Node> Nodes;

            explicit Node(Astrelis::File file) : File(std::move(file)) {}

            bool operator==(const Node& other) const
            {
                return File == other.File;
            }
        };

        explicit FileTree(Astrelis::File root);

        Astrelis::RawRef<Node*> ImGuiTree();

    private:
        Node m_Root;
    };
} // namespace Pulsar
