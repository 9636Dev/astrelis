#pragma once

#include "Lexer.hpp"

#include <vector>

namespace Nebula::Shader
{
    class Parser
    {
    public:
        explicit Parser(Lexer lexer) noexcept : m_Lexer(std::move(lexer)) {}

        ~Parser()                            = default;
        Parser(const Parser&)                = delete;
        Parser& operator=(const Parser&)     = delete;
        Parser(Parser&&) noexcept            = delete;
        Parser& operator=(Parser&&) noexcept = delete;

    private:
        class BacktrackableLexer
        {
        public:
            explicit BacktrackableLexer(Lexer lexer) noexcept : m_Lexer(std::move(lexer)) {}

            ~BacktrackableLexer() noexcept                               = default;
            BacktrackableLexer(const BacktrackableLexer&)                = delete;
            BacktrackableLexer& operator=(const BacktrackableLexer&)     = delete;
            BacktrackableLexer(BacktrackableLexer&&) noexcept            = delete;
            BacktrackableLexer& operator=(BacktrackableLexer&&) noexcept = delete;

            Token Peek(std::size_t offset = 0);
            Token Read();
            void Consume(std::size_t count = 1);
            bool Consume(TokenType type);
            bool Consume(TokenType type, Token& token);
            void Backtrack(std::size_t count = 1);
            void BacktrackTo(std::size_t index);
            void GoTo(std::size_t index);

            std::size_t GetCurrentIndex() const { return CurrentTokenIndex; }
        private:
            void ReadTokens(std::size_t count);

            Lexer m_Lexer;
            std::size_t CurrentTokenIndex = 0;
            std::vector<Token> TokensRead;
            std::size_t EndOfStreamIndex = 0;
        };

        BacktrackableLexer m_Lexer;
    };
} // namespace Nebula::Shader

