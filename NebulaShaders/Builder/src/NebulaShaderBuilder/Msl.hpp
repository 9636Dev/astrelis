#pragma once

#include "Core.hpp"
#include "NebulaIO/File.hpp"
#include "NebulaShaderCommon/Bindings.hpp"
#include "NebulaShaderCommon/Inputs.hpp"
#include <vector>

namespace Nebula::Shader
{
    struct NEBULA_SHADER_BUILDER_API Msl
    {
        std::string Name;
        std::vector<Input> Inputs;
        std::vector<Input> PixelInputs;
        std::vector<UniformBuffer> UniformBuffers;
        std::vector<TextureBinding> Textures;
        std::string VertexSource;
        std::string PixelSource;

        static std::optional<Msl> FromSource(const std::string& source);
        static std::optional<Msl> FromFile(const Nebula::File& file);

        Msl() = default;

        Msl(std::string name,
            std::vector<Input> inputs,
            std::vector<Input> pixelInputs,
            std::vector<UniformBuffer> uniformBuffers,
            std::vector<TextureBinding> textures,
            std::string vertexSource,
            std::string pixelSource) :
            Name(std::move(name)),
            Inputs(std::move(inputs)),
            PixelInputs(std::move(pixelInputs)),
            UniformBuffers(std::move(uniformBuffers)),
            Textures(std::move(textures)),
            VertexSource(std::move(vertexSource)),
            PixelSource(std::move(pixelSource))
        {
        }
    };
} // namespace Nebula::Shader
