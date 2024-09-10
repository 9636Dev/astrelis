#pragma once

#include "FileTree.hpp"

#include "Astrelis/IO/File.hpp"

#include <string>

namespace Pulsar
{
class AssetPanel
{
public:
    explicit AssetPanel(Astrelis::File rootDirectory);

    void Draw();
private:
    FileTree m_FileTree;
};
}
