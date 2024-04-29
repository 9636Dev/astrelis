#pragma once

#include <cstdint>
#include <string>

#include "NebulaShaderCommon/ShaderMeta.hpp"
#include "NebulaShaderCommon/ShaderSource.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Nebula::Shader
{

    struct FileHeader
    {
        std::string Identifier = ".NSL";
        std::uint32_t Version  = 1;
        ShaderProgram Program;
        bool GlslPresent = false;

        template<typename Archive> void serialize(Archive& archive) { archive(Identifier, Version, Program, GlslPresent); }
    };

    using GLSLSource = GLSLShaderSource;
} // namespace Nebula::Shader
