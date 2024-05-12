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

        /**
         * @brief Load the default assets for the engine, such as shaders and textures
         *
         * @return true if the assets were loaded successfully
         * @return false if the assets were not loaded successfully
         */
        virtual bool LoadDefaultAssets() = 0;
        virtual bool LoadShader(const File& file)  = 0;
        virtual bool LoadTexture(const Texture& texture) = 0; // We should move the texture here, and never copy it
    };
} // namespace Nebula
