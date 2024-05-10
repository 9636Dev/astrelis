#include "Msl.hpp"
#include "NebulaShaderBuilder/FileToOutput.hpp"

namespace Nebula::Shader
{
    inline static std::optional<Msl> FromIntermediate(IntermediateOutput output)
    {
        if (!output.MslSource.has_value())
        {
            return std::nullopt;
        }

        std::string name                     = output.Header.Program.Meta.Name;
        auto inputs                          = output.Header.Program.Vertex.Inputs;
        auto pixelInputs                     = output.Header.Program.Fragment.Inputs;
        std::vector<Binding> bindings        = output.Header.Program.Meta.Bindings;
        std::vector<TextureBinding> textures = output.Header.Program.Meta.Textures;

        Msl msl;
        msl.Name         = name;
        msl.Bindings     = bindings;
        msl.Textures     = textures;
        msl.Inputs       = inputs;
        msl.PixelInputs  = pixelInputs;
        msl.VertexSource = output.MslSource.value().VertexSource;
        msl.PixelSource  = output.MslSource.value().PixelSource;

        return msl;
    }

    std::optional<Msl> Msl::FromFile(const Nebula::File& file)
    {
        IntermediateOutput output = IntermediateOutput::FromFile(file);
        return FromIntermediate(output);
    }

    std::optional<Msl> Msl::FromSource(const std::string& source)
    {
        IntermediateOutput output = IntermediateOutput::FromSource(source);
        return FromIntermediate(output);
    }
} // namespace Nebula::Shader

