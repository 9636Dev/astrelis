#pragma once

#include "Bindings.hpp"
#include "Inputs.hpp"

#include <string>
#include <vector>

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
    };

    /**
    * @brief Meta about a single shader stage
    */
    struct ShaderMeta
    {
        std::string Entrypoint;
        std::vector<Input> Inputs;
    };

    struct ShaderProgram
    {
        ProgramMeta Meta;
        ShaderMeta Vertex;
        ShaderMeta Fragment;

        ShaderProgram(ProgramMeta meta, ShaderMeta vertex, ShaderMeta fragment) :
            Meta(std::move(meta)),
            Vertex(std::move(vertex)),
            Fragment(std::move(fragment))
        {
        }
    };
} // namespace Nebula::Shader
