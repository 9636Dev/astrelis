#pragma once

#include "Core.hpp"
#include "Token.hpp"

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
        Lexer(Lexer&&) noexcept            = default;
        Lexer& operator=(Lexer&&) noexcept = default;

        // Get the next token from the source
        [[nodiscard]] Token NextToken() noexcept;

    private:
        Token LexStringLiteral() noexcept;
        Token LexNumberLiteral() noexcept;
        Token LexSymbol() noexcept;

        // Get the current character from the source
        [[nodiscard]] char CurrentChar() const noexcept;
        // Peek at the next character without advancing the lexer
        [[nodiscard]] char PeekChar() const noexcept;
        // Advance the lexer to the next character
        void Advance() noexcept;
        // Skip any whitespace characters (C standard for whitespace)
        void SkipWhitespace() noexcept;

        [[nodiscard]] char CheckChar(char character) const noexcept;

        enum class LexerState
        {
            NoError,      // No error
            InvalidAscii, // Invalid ASCII character
            EndOfFile,    // End of file
        };

        std::size_t m_Index        = 0;
        mutable LexerState m_State = LexerState::NoError;
        std::string_view m_Source;
    };
} // namespace Nebula::Shader
