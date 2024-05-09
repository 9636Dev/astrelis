#pragma once

#include "NebulaIO/File.hpp"
#include "NebulaShaderCommon/Bindings.hpp"
#include "NebulaShaderCommon/Inputs.hpp"
#include <vector>
namespace Nebula::Shader
{
    struct Msl
    {
        std::vector<Input> Inputs;
        std::vector<Input> PixelInputs;
        std::vector<Binding> Bindings;
        std::vector<TextureBinding> Textures;
        std::string VertexSource;
        std::string PixelSource;

        static std::optional<Msl> FromSource(const std::string& source);
        static std::optional<Msl> FromFile(const Nebula::File& file);

        Msl() = default;
        Msl(std::vector<Input> inputs,
            std::vector<Input> pixelInputs,
            std::vector<Binding> bindings,
            std::vector<TextureBinding> textures,
            std::string vertexSource,
            std::string pixelSource) :
            Inputs(std::move(inputs)),
            PixelInputs(std::move(pixelInputs)),
            Bindings(std::move(bindings)),
            Textures(std::move(textures)),
            VertexSource(std::move(vertexSource)),
            PixelSource(std::move(pixelSource))
        {
        }
    };
} // namespace Nebula::Shader
