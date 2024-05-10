#pragma once

#include "Core.hpp"
#include "NebulaIO/File.hpp"
#include "NebulaShaderBuilder/File.hpp"

namespace Nebula::Shader
{
    struct NEBULA_SHADER_BUILDER_API IntermediateOutput
    {
        FileHeader Header;
        std::optional<GLSLSource> GlslSource;
        std::optional<MSLSource> MslSource;

        static IntermediateOutput FromFile(const File& file);
        static IntermediateOutput FromSource(const std::string& source);
    };
} // namespace Nebula::Shader
