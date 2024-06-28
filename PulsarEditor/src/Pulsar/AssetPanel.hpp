#pragma once

#include "FileTree.hpp"

#include "NebulaEngine/IO/File.hpp"

#include <string>

namespace Pulsar
{
class AssetPanel
{
public:
    explicit AssetPanel(Nebula::File rootDirectory);

    void Draw();
private:
    FileTree m_FileTree;
};
}
