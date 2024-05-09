#pragma once

#include "NebulaIO/File.hpp"
#include "NebulaShaderBuilder/File.hpp"

namespace Nebula::Shader
{
    struct IntermediateOutput
    {
        FileHeader Header;
        std::optional<GLSLSource> GlslSource;

        static IntermediateOutput FromFile(const File& file);
        static IntermediateOutput FromSource(const std::string& source);
    };
} // namespace Nebula::Shader
