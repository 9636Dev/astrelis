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
        static constexpr auto VERSION = 1;
        std::string Identifier        = ".NSL";
        std::uint32_t Version         = VERSION;
        ShaderProgram Program;
        bool GlslPresent = false;
        bool MslPresent  = false;

        template<typename Archive> void serialize(Archive& archive)
        {
            archive(Identifier, Version);

            if (Version != VERSION)
            {
                throw std::runtime_error("Invalid version");
            }

            archive(Program, GlslPresent, MslPresent);
        }
    };

    using GLSLSource = GLSLShaderSource;

    struct MSLSource
    {
        std::string VertexSource;
        std::string PixelSource;

        MSLSource() = default;
        MSLSource(std::string vertexSource, std::string pixelSource)
            : VertexSource(std::move(vertexSource)), PixelSource(std::move(pixelSource))
        {
        }

        template<typename Archive> void serialize(Archive& archive)
        {
            archive(VertexSource, PixelSource);
        }
    };
} // namespace Nebula::Shader
