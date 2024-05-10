#include "FileToOutput.hpp"

#include <fstream>

namespace Nebula::Shader
{
    template<typename T>
        requires std::derived_from<T, std::istream>
    static IntermediateOutput ReadFromStream(T& stream)
    {
        IntermediateOutput output;

        cereal::BinaryInputArchive archive(stream);
        FileHeader header;
        archive(header);
        output.Header = header;

        if (header.GlslPresent)
        {
            GLSLSource glslSource;
            archive(glslSource);
            output.GlslSource = glslSource;
        }

        if (header.MslPresent)
        {
            MSLSource mslSource;
            archive(mslSource);
            output.MslSource = mslSource;
        }

        return output;
    }

    IntermediateOutput IntermediateOutput::FromFile(const File& file)
    {
        std::ifstream input(file.GetPath(), std::ios::binary);
        if (!input.good())
        {
            IntermediateOutput output;
            output.Header.Identifier.clear();
            return output;
        }

        return ReadFromStream(input);
    }

    IntermediateOutput IntermediateOutput::FromSource(const std::string& source)
    {
        std::stringstream input(source);
        if (!input.good())
        {
            IntermediateOutput output;
            output.Header.Identifier.clear();
            return output;
        }

        return ReadFromStream(input);
    }
} // namespace Nebula::Shader
