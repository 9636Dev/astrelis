#include "FileToOutput.hpp"

#include <fstream>

namespace Nebula::Shader
{
    IntermediateOutput IntermediateOutput::FromFile(const File& file)
    {
        IntermediateOutput output;

        std::ifstream input(file.GetPath(), std::ios::binary);
        if (!input.good())
        {
            output.Header.Identifier.clear();
            return output;
        }

        cereal::BinaryInputArchive archive(input);
        FileHeader header;
        archive(header);
        output.Header = header;

        if (header.GlslPresent)
        {
            GLSLSource glslSource;
            archive(glslSource);
            output.GlslSource = glslSource;
        }

        return output;
    }

    IntermediateOutput IntermediateOutput::FromSource(const std::string& source)
    {
        IntermediateOutput output;

        std::stringstream input(source);
        cereal::BinaryInputArchive archive(input);

        FileHeader header;
        archive(header);
        output.Header = header;

        if (header.GlslPresent)
        {
            GLSLSource glslSource;
            archive(glslSource);
            output.GlslSource = glslSource;
        }

        return output;
    }
} // namespace Nebula::Shader
