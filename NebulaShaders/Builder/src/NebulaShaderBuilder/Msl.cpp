#include "Msl.hpp"
#include "NebulaShaderBuilder/FileToOutput.hpp"

namespace Nebula::Shader
{
    inline static std::optional<Msl> FromIntermediate(IntermediateOutput output)
    {
        if (!output.GlslSource.has_value())
        {
            return std::nullopt;
        }

        std::string name                     = output.Header.Program.Meta.Name;
        auto inputs                          = output.Header.Program.Vertex.Inputs;
        auto pixelInputs                     = output.Header.Program.Fragment.Inputs;
        std::vector<Binding> bindings        = output.Header.Program.Meta.Bindings;
        std::vector<TextureBinding> textures = output.Header.Program.Meta.Textures;
    }
} // namespace Nebula::Shader

