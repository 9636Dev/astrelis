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

            UniformBuffer() = default;

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

            Sampler() = default;

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

    struct MSLMeta
    {
        struct Binding
        {
            std::string Name;
            std::uint32_t Index = 0;

            Binding() = default;

            Binding(std::string name, std::uint32_t index) : Name(std::move(name)), Index(index) {}

            ~Binding()                         = default;
            Binding(const Binding&)            = default;
            Binding(Binding&&)                 = default;
            Binding& operator=(const Binding&) = default;
            Binding& operator=(Binding&&)      = default;

            bool operator==(const Binding& other) const { return Name == other.Name && Index == other.Index; }

            template<typename Archive> void serialize(Archive& archive) { archive(Name, Index); }
        };

        struct Texture
        {
            std::string Name;
            std::uint32_t Index = 0;

            Texture() = default;

            Texture(std::string name, std::uint32_t index) : Name(std::move(name)), Index(index) {}

            ~Texture()                         = default;
            Texture(const Texture&)            = default;
            Texture(Texture&&)                 = default;
            Texture& operator=(const Texture&) = default;
            Texture& operator=(Texture&&)      = default;

            bool operator==(const Texture& other) const { return Name == other.Name && Index == other.Index; }

            template<typename Archive> void serialize(Archive& archive) { archive(Name, Index); }
        };

        struct Sampler
        {
            std::string Name;
            std::uint32_t Index = 0;

            Sampler() = default;

            Sampler(std::string name, std::uint32_t index) : Name(std::move(name)), Index(index) {}

            ~Sampler()                         = default;
            Sampler(const Sampler&)            = default;
            Sampler(Sampler&&)                 = default;
            Sampler& operator=(const Sampler&) = default;
            Sampler& operator=(Sampler&&)      = default;

            bool operator==(const Sampler& other) const { return Name == other.Name && Index == other.Index; }

            template<typename Archive> void serialize(Archive& archive) { archive(Name, Index); }
        };

        std::map<std::string, Binding> Bindings;
        std::map<std::string, Texture> Textures;
        std::map<std::string, Sampler> Samplers;

        MSLMeta() = default;

        bool operator==(const MSLMeta& other) const
        {
            return Bindings == other.Bindings && Textures == other.Textures && Samplers == other.Samplers;
        }

        template<typename Archive> void serialize(Archive& archive) { archive(Bindings, Textures, Samplers); }
    };
} // namespace Nebula::Shader
