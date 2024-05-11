#include "Glsl.hpp"

#include "File.hpp"
#include "FileToOutput.hpp"

namespace Nebula::Shader
{
    inline static std::optional<Glsl> FromIntermediate(IntermediateOutput output)
    {
        if (!output.GlslSource.has_value())
        {
            return std::nullopt;
        }

        std::string name = output.Header.Program.Meta.Name;
        auto inputs      = output.Header.Program.Vertex.Inputs;
        auto outputs     = output.Header.Program.Fragment.Inputs;

        std::vector<Glsl::UniformBuffer> uniformBuffers;
        std::vector<Glsl::Texture> textures;
        {
            auto programUniformBuffers = output.Header.Program.Meta.UniformBuffers;

            for (auto& ubo : programUniformBuffers)
            {
                std::vector<Glsl::Binding> uboBindings;
                for (auto& binding : ubo.Bindings)
                {
                    uboBindings.emplace_back(binding.Type, binding.Name, binding.Target);
                }

                auto slot = output.GlslSource->Meta.UniformBuffers[ubo.Name].Binding;
                uniformBuffers.emplace_back(ubo.Name, std::move(uboBindings), slot);
            }
        }

        {
            auto programTextures = output.Header.Program.Meta.Textures;

            std::map<std::string, std::pair<TextureType, TextureTarget>> textureMap;
            for (auto& texture : programTextures)
            {
                textureMap[texture.Name] = std::make_pair(texture.Type, texture.Target);
            }

            auto glslTextures = output.GlslSource->Meta.Samplers;
            for (auto& glslTexture : glslTextures)
            {
                auto type = textureMap[glslTexture.first];
                textures.emplace_back(type.first, glslTexture.second.Name, type.second, glslTexture.second.Binding);
            }
        }

        return Glsl(name, output.GlslSource->Version, output.GlslSource->Glsl420Pack, std::move(inputs),
                    std::move(outputs), std::move(uniformBuffers), std::move(textures), output.GlslSource->VertexSource,
                    output.GlslSource->PixelSource);
    }

    std::optional<Glsl> Glsl::FromFile(const Nebula::File& file)
    {
        IntermediateOutput output = IntermediateOutput::FromFile(file);
        return FromIntermediate(output);
    }

    std::optional<Glsl> Glsl::FromSource(const std::string& source)
    {
        IntermediateOutput output = IntermediateOutput::FromSource(source);
        return FromIntermediate(output);
    }
} // namespace Nebula::Shader
