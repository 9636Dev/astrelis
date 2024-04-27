#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>

#include <cereal/archives/binary.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/string.hpp>

namespace Nebula::Shader
{
    struct GLSLMeta
    {
        struct UniformBuffer
        {
            std::string Name; // New uniform buffer name
            std::optional<std::uint32_t> Binding;

            UniformBuffer(std::string name, std::optional<std::uint32_t> binding) :
                Name(std::move(name)),
                Binding(binding)
            {
            }

            ~UniformBuffer()                               = default;
            UniformBuffer(const UniformBuffer&)            = default;
            UniformBuffer(UniformBuffer&&)                 = default;
            UniformBuffer& operator=(const UniformBuffer&) = default;
            UniformBuffer& operator=(UniformBuffer&&)      = default;

            bool operator==(const UniformBuffer& other) const { return Name == other.Name && Binding == other.Binding; }

            template<typename Archive> void serialize(Archive& archive) { archive(Name, Binding); }
        };

        struct Sampler
        {
            std::string Name; // New sampler name
            std::optional<std::uint32_t> Binding;

            Sampler(std::string name, std::optional<std::uint32_t> binding) : Name(std::move(name)), Binding(binding) {}

            ~Sampler()                         = default;
            Sampler(const Sampler&)            = default;
            Sampler(Sampler&&)                 = default;
            Sampler& operator=(const Sampler&) = default;
            Sampler& operator=(Sampler&&)      = default;

            bool operator==(const Sampler& other) const { return Name == other.Name && Binding == other.Binding; }

            template<typename Archive> void serialize(Archive& archive) { archive(Name, Binding); }
        };

        // Original uniform buffer name -> uniform buffer
        std::map<std::string, UniformBuffer> UniformBuffers;
        // Original sampler name -> sampler
        std::map<std::string, Sampler> Samplers;

        GLSLMeta() = default;

        bool operator==(const GLSLMeta& other) const
        {
            return UniformBuffers == other.UniformBuffers && Samplers == other.Samplers;
        }

        template<typename Archive> void serialize(Archive& archive) { archive(UniformBuffers, Samplers); }
    };

    struct HLSLMeta
    {
    };

    struct MetalMeta
    {
    };
} // namespace Nebula::Shader
