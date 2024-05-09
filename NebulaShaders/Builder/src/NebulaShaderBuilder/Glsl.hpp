#pragma once

// Include this file to be able to use the Intermediate file to Glsl conversion

#include "Core.hpp"
#include "NebulaIO/File.hpp"

#include "NebulaShaderCommon/Bindings.hpp"
#include "NebulaShaderCommon/Inputs.hpp"

namespace Nebula::Shader
{
    struct NEBULA_SHADER_BUILDER_API Glsl
    {
        struct Binding
        {
            BindingType Type;
            std::string Name;
            std::optional<std::uint32_t> Slot;

            Binding(BindingType type, std::string name, std::optional<std::uint32_t> slot) :
                Type(type),
                Name(std::move(name)),
                Slot(slot)
            {
            }
        };

        struct Texture
        {
            TextureType Type;
            std::string Name;
            std::optional<std::uint32_t> Slot;

            Texture(TextureType type, std::string name, std::optional<std::uint32_t> slot) :
                Type(type),
                Name(std::move(name)),
                Slot(slot)
            {
            }
        };

        std::string Name;
        std::uint32_t GlslVersion {};
        bool Glsl420PackEnabled {};
        std::vector<Input> Inputs;
        std::vector<Input> PixelInputs;
        std::vector<Binding> Bindings;
        std::vector<Texture> Textures;
        std::string VertexSource;
        std::string PixelSource;

        static std::optional<Glsl> FromSource(const std::string& source);
        static std::optional<Glsl> FromFile(const Nebula::File& file);

        Glsl(std::string name,
             std::uint32_t glslVersion,
             bool glsl420PackEnabled,
             std::vector<Input> inputs,
             std::vector<Input> pixelInputs,
             std::vector<Binding> bindings,
             std::vector<Texture> textures,
             std::string vertexSource,
             std::string pixelSource) :
            Name(std::move(name)),
            GlslVersion(glslVersion),
            Glsl420PackEnabled(glsl420PackEnabled),
            Inputs(std::move(inputs)),
            PixelInputs(std::move(pixelInputs)),
            Bindings(std::move(bindings)),
            Textures(std::move(textures)),
            VertexSource(std::move(vertexSource)),
            PixelSource(std::move(pixelSource))
        {
        }

        Glsl() = default;
    };
} // namespace Nebula::Shader
