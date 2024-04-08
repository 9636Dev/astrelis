#pragma once

#include "Core.hpp"

#include <string_view>

namespace Nebula::Shader
{
    class NEBULA_SHADER_COMPILER_API Lexer
    {
    public:
        explicit Lexer(std::string_view source) noexcept;
        ~Lexer() noexcept = default;
        Lexer(const Lexer&);
        Lexer& operator=(const Lexer&);
        Lexer(Lexer&&) noexcept = default;
        Lexer& operator=(Lexer&&) noexcept = default;


    private:
        std::string_view m_Source;
    };
} // namespace Nebula::Shader
