#pragma once

#include "Core.hpp"
#include <string>
#include <string_view>

namespace Nebula::Shader
{
    enum class TokenType
    {
        EndOfFile,
        Invalid,

        // Keywords
        NameKeyword,
        BindingsKeyword,
        UniformsKeyword,
        TexturesKeyword,
        InputKeyword,
        FragmentInputKeyword,
        ShaderKeyword,
        SharedKeyword,
        VertexKeyword,
        FragmentKeyword,
        GeneratedKeyword,
        EntrypointKeyword,

        // Based on ASCII order
        ExclamationMark,
        ExclamationMarkEqual,
        StringLiteral,
        InvalidStringLiteral,
        Hash,
        Dollar,
        Percent,
        Ampersand,
        Apostrophe,
        CharacterLiteral,
        LeftParenthesis,
        RightParenthesis,
        Asterisk,
        Plus,
        Comma,
        Minus,
        Period,
        Slash,
        Comment,
        BlockComment,
        InvalidComment,
        NumberLiteral,
        HexNumberLiteral,
        BinaryNumberLiteral,
        OctalNumberLiteral,
        InvalidNumberLiteral,
        Colon,
        ColonColon,
        Semicolon,
        LessThan,
        LessThanEqual,
        LessThanLessThan,
        LessThanLessThanEqual,
        Equal,
        EqualEqual,
        GreaterThan,
        GreaterThanEqual,
        GreaterThanGreaterThan,
        GreaterThanGreaterThanEqual,
        QuestionMark,
        AtSign,
        Symbol,
        LeftBracket,
        Backslash,
        RightBracket,
        Caret,
        Underscore,
        Backtick,
        LeftBrace,
        VerticalBar,
        RightBrace,
        Tilde,
    };

    struct NEBULA_SHADER_COMPILER_API Token
    {
        std::string_view Text;
        std::size_t Index = 0;
        TokenType Type    = TokenType::EndOfFile;

        Token() noexcept = default;

        Token(std::string_view text, std::size_t index, TokenType type) noexcept : Text(text), Index(index), Type(type)
        {
        }

        [[nodiscard]] bool operator==(const Token& other) const noexcept;
        [[nodiscard]] bool operator!=(const Token& other) const noexcept;

        static std::string TypeString(TokenType type) noexcept;
        [[nodiscard]] std::string ToString() const;
    };
} // namespace Nebula::Shader
