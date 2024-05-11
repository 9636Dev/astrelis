#pragma once

#include "NebulaIO/File.hpp"
#include "Texture.hpp"

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
        virtual bool LoadTexture(const Texture& texture) = 0; // We should move the texture here, and never copy it
    };
} // namespace Nebula
