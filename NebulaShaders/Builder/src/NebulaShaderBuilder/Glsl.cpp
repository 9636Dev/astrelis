#include "Glsl.hpp"

#include "FileToOutput.hpp"

namespace Nebula::Shader
{
    std::optional<Glsl> Glsl::FromFile(const Nebula::File& file)
    {
        IntermediateOutput output = IntermediateOutput::FromFile(file);

        if (!output.GlslSource.has_value())
        {
            return std::nullopt;
        }

        std::string name = output.Header.Program.Meta.Name;
        auto inputs = output.Header.Program.Vertex.Inputs;
        auto outputs = output.Header.Program.Fragment.Inputs;

        std::vector<Binding> bindings;
        std::vector<Texture> textures;
        {
            auto programBindings = output.Header.Program.Meta.Bindings;

            std::map<std::string, BindingType> bindingMap;
            for (auto& binding : programBindings)
            {
                bindingMap[binding.Name] = binding.Type;
            }

            auto glslBindings = output.GlslSource->Meta.UniformBuffers;
            for (auto& glslBinding : glslBindings)
            {
                auto type = bindingMap[glslBinding.first];
                bindings.emplace_back(type, glslBinding.second.Name, glslBinding.second.Binding);
            }
        }

        {
            auto programTextures = output.Header.Program.Meta.Textures;

            std::map<std::string, TextureType> textureMap;
            for (auto& texture : programTextures)
            {
                textureMap[texture.Name] = texture.Type;
            }

            auto glslTextures = output.GlslSource->Meta.Samplers;
            for (auto& glslTexture : glslTextures)
            {
                auto type = textureMap[glslTexture.first];
                textures.emplace_back(type, glslTexture.second.Name, glslTexture.second.Binding);
            }
        }

        return Glsl(name,
                    std::move(inputs),
                    std::move(outputs),
                    std::move(bindings),
                    std::move(textures),
                    output.GlslSource->VertexSource,
                    output.GlslSource->PixelSource);
    }
} // namespace Nebula::Shader
