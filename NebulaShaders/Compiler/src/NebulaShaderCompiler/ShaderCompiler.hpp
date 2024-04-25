#pragma once

#include "Core.hpp"

#include <array>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace Nebula::Shader
{
    class NEBULA_SHADER_COMPILER_API Compiler
    {
    public:
        struct Error
        {
            std::string Message;
            std::size_t Index;
        };

        struct Binding
        {
            std::string Type;
            std::string Name;
            std::string Binding;
        };

        struct Meta
        {
            std::string Name;
            std::vector<Binding> Uniforms;
            std::vector<Binding> Textures;
            std::vector<Binding> Inputs;
            std::vector<Binding> FragmentInputs;
            std::string VertexEntrypoint;
            std::string FragmentEntrypoint;
        };

        struct Sources
        {
            std::optional<std::string> Shared = std::nullopt;
            std::optional<std::string> Vertex = std::nullopt;
            std::optional<std::string> Fragment = std::nullopt;
        };

        explicit Compiler(std::string source) : m_Source(std::move(source)) {}

        Compiler(const Compiler&)                = default;
        Compiler(Compiler&&) noexcept            = default;
        Compiler& operator=(const Compiler&)     = default;
        Compiler& operator=(Compiler&&) noexcept = default;
        ~Compiler()                              = default;

        std::optional<Error> Compile();
        [[nodiscard]] std::array<std::size_t, 2> GetRowColumn(std::size_t index) const;

        [[nodiscard]] const Meta& GetMeta() const { return m_Meta; }

        [[nodiscard]] const Sources& GetSources() const { return m_Sources; }
    private:
        Meta m_Meta;
        Sources m_Sources;
        std::string m_Source;
    };
} // namespace Nebula::Shader
