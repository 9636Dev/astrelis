#pragma once

#include "Bindings.hpp"
#include "Inputs.hpp"

#include <string>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

namespace Nebula::Shader
{
    /**
    * Meta about the whole shader program (vertex + fragment)
    */
    struct ProgramMeta
    {
        std::string Name;
        std::vector<Binding> Bindings;
        std::vector<TextureBinding> Textures;


        ProgramMeta() = default;

        template<typename Archive> void serialize(Archive& archive)
        {
            archive(Name, Bindings, Textures);
        }
    };

    /**
    * @brief Meta about a single shader stage
    */
    struct ShaderMeta
    {
        std::string Entrypoint;
        std::vector<Input> Inputs;

        ShaderMeta() = default;
        ShaderMeta(std::string entrypoint, std::vector<Input> inputs) :
            Entrypoint(std::move(entrypoint)),
            Inputs(std::move(inputs))
        {
        }

        template<typename Archive> void serialize(Archive& archive)
        {
            archive(Entrypoint, Inputs);
        }
    };

    struct ShaderProgram
    {
        ProgramMeta Meta;
        ShaderMeta Vertex;
        ShaderMeta Fragment;

        ShaderProgram() = default;

        ShaderProgram(ProgramMeta meta, ShaderMeta vertex, ShaderMeta fragment) :
            Meta(std::move(meta)),
            Vertex(std::move(vertex)),
            Fragment(std::move(fragment))
        {
        }

        template<typename Archive> void serialize(Archive& archive)
        {
            archive(Meta, Vertex, Fragment);
        }
    };
} // namespace Nebula::Shader
