#pragma once

#include "Astrelis/IO/File.hpp"

#include <string>

#include "FileTree.hpp"

namespace AstrelisEditor {
    class AssetPanel {
    public:
        explicit AssetPanel(Astrelis::File rootDirectory);

        void Draw();
    private:
        FileTree m_FileTree;
    };
} // namespace AstrelisEditor
