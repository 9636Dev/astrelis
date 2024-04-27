#pragma once

#include <cstdint>
#include <string>

#include "NebulaShaderCommon/ShaderSource.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Nebula::Shader
{

    struct FileHeader
    {
        std::string Identifier = ".NSL";
        std::uint32_t Version  = 1;
        std::string Name;
        bool GlslPresent = false;

        template<typename Archive> void serialize(Archive& archive) { archive(Identifier, Version, Name, GlslPresent); }
    };

    struct GLSLSource : GLSLShaderSource
    {
        template<typename Archive> void serialize(Archive& archive)
        {
            archive(Version, Meta, VertexSource, PixelSource);
        }
    };
} // namespace Nebula::Shader
