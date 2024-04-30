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
        bool Glsl420Pack = false;
        GLSLMeta Meta;
        std::string VertexSource;
        std::string PixelSource;

        template <typename Archive>
        void serialize(Archive& archive)
        {
            archive(Version, Glsl420Pack, Meta, VertexSource, PixelSource);
        }
    };

    struct MSLShaderSource
    {
        MSLShaderSource() = default;

        std::uint32_t Version = 2;
        //MSLMeta Meta;
    };
} // namespace Nebula::Shader
