#pragma once

#include <string>

class AssetPanel
{
public:
    explicit AssetPanel(std::string rootDirectory);

    void Draw();
private:
    std::string m_RootDirectory;
};
