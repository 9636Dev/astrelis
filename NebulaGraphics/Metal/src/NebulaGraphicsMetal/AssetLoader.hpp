#pragma once

#include "NebulaGraphicsCore/AssetLoader.hpp"
#include "NebulaShaderBuilder/Msl.hpp"

#include <unordered_map>

#include <Metal/Metal.H>

namespace Nebula
{
    class MetalAssetLoader : public AssetLoader
    {
    public:
        explicit MetalAssetLoader(id<MTLDevice> device);
        ~MetalAssetLoader() override                      = default;
        MetalAssetLoader(const MetalAssetLoader&)            = default;
        MetalAssetLoader(MetalAssetLoader&&)                 = default;
        MetalAssetLoader& operator=(const MetalAssetLoader&) = default;
        MetalAssetLoader& operator=(MetalAssetLoader&&)      = default;

        bool LoadShader(const File& file) override;

        // Assesed through DLL, so we can make this public
        struct MslShader
        {
            Shader::Msl Shader;
            id<MTLFunction> Vertex;
            id<MTLFunction> Pixel;
        };
        std::unordered_map<std::string, MslShader> m_Shaders;

        id<MTLDevice> m_Device;
    };
} // namespace Nebula
