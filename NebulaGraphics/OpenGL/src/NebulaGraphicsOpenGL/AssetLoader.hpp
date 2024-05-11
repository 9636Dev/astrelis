#pragma once

#include "NebulaGraphicsCore/AssetLoader.hpp"
#include "NebulaGraphicsOpenGL/OpenGL/Texture.hpp"
#include "NebulaShaderBuilder/Glsl.hpp"

#include <unordered_map>

namespace Nebula
{
    class GlAssetLoader : public AssetLoader
    {
    public:
        GlAssetLoader()                                = default;
        ~GlAssetLoader() override                      = default;
        GlAssetLoader(const GlAssetLoader&)            = default;
        GlAssetLoader(GlAssetLoader&&)                 = default;
        GlAssetLoader& operator=(const GlAssetLoader&) = default;
        GlAssetLoader& operator=(GlAssetLoader&&)      = default;

        bool LoadShader(const File& file) override;
        bool LoadTexture(const Texture& texture) override;

        // Assesed through DLL, so we can make this public
        std::unordered_map<std::string, Shader::Glsl> m_Shaders;
        std::unordered_map<std::string, OpenGL::Texture> m_Textures;
    };
} // namespace Nebula
