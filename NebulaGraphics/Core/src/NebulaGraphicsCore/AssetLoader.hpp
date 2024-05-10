#pragma once

#include "NebulaIO/File.hpp"

namespace Nebula
{
    class AssetLoader
    {
    public:
        AssetLoader()                              = default;
        virtual ~AssetLoader()                     = default;
        AssetLoader(const AssetLoader&)            = default;
        AssetLoader(AssetLoader&&)                 = default;
        AssetLoader& operator=(const AssetLoader&) = default;
        AssetLoader& operator=(AssetLoader&&)      = default;
        virtual bool LoadShader(const File& file)  = 0;
    };
} // namespace Nebula
