#pragma once

#include <string>

#include "Meta.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Nebula::Shader
{
    struct GLSLShaderSource
    {
        GLSLShaderSource() = default;

        std::uint32_t Version = 450;
        GLSLMeta Meta;
        std::string VertexSource;
        std::string PixelSource;

        template <typename Archive>
        void serialize(Archive& archive)
        {
            archive(Version, Meta, VertexSource, PixelSource);
        }
    };
} // namespace Nebula::Shader
