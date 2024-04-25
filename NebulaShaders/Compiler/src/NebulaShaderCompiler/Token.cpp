#include "Token.hpp"
#include <sstream>

namespace Nebula::Shader
{
    bool Token::operator==(const Token& other) const noexcept { return Type == other.Type && Index == other.Index; }

    bool Token::operator!=(const Token& other) const noexcept { return !(*this == other); }

    std::string Token::TypeString(TokenType type) noexcept
    {
        switch (type)
        {
        case TokenType::EndOfFile:
            return "EndOfFile";
        case TokenType::Invalid:
            return "Invalid";
        case TokenType::NameKeyword:
            return "NameKeyword";
        case TokenType::BindingsKeyword:
            return "BindingsKeyword";
        case TokenType::UniformsKeyword:
            return "UniformsKeyword";
        case TokenType::TexturesKeyword:
            return "TexturesKeyword";
        case TokenType::InputKeyword:
            return "InputKeyword";
        case TokenType::FragmentInputKeyword:
            return "PixelInputKeyword";
        case TokenType::ShaderKeyword:
            return "ShaderKeyword";
        case TokenType::SharedKeyword:
            return "SharedKeyword";
        case TokenType::VertexKeyword:
            return "VertexKeyword";
        case TokenType::FragmentKeyword:
            return "FragmentKeyword";
        case TokenType::GeneratedKeyword:
            return "GeneratedKeyword";
        case TokenType::EntrypointKeyword:
            return "EntrypointKeyword";
        case TokenType::ExclamationMark:
            return "ExclamationMark";
        case TokenType::ExclamationMarkEqual:
            return "ExclamationMarkEqual";
        case TokenType::StringLiteral:
            return "StringLiteral";
        case TokenType::InvalidStringLiteral:
            return "InvalidStringLiteral";
        case TokenType::Hash:
            return "Hash";
        case TokenType::Dollar:
            return "Dollar";
        case TokenType::Percent:
            return "Percent";
        case TokenType::Ampersand:
            return "Ampersand";
        case TokenType::Apostrophe:
            return "Apostrophe";
        case TokenType::CharacterLiteral:
            return "CharacterLiteral";
        case TokenType::LeftParenthesis:
            return "LeftParenthesis";
        case TokenType::RightParenthesis:
            return "RightParenthesis";
        case TokenType::Asterisk:
            return "Asterisk";
        case TokenType::Plus:
            return "Plus";
        case TokenType::Comma:
            return "Comma";
        case TokenType::Minus:
            return "Minus";
        case TokenType::Period:
            return "Period";
        case TokenType::Slash:
            return "Slash";
        case TokenType::Comment:
            return "Comment";
        case TokenType::BlockComment:
            return "BlockComment";
        case TokenType::InvalidComment:
            return "InvalidComment";
        case TokenType::NumberLiteral:
            return "NumberLiteral";
        case TokenType::HexNumberLiteral:
            return "HexNumberLiteral";
        case TokenType::BinaryNumberLiteral:
            return "BinaryNumberLiteral";
        case TokenType::OctalNumberLiteral:
            return "OctalNumberLiteral";
        case TokenType::InvalidNumberLiteral:
            return "InvalidNumberLiteral";
        case TokenType::Colon:
            return "Colon";
        case TokenType::ColonColon:
            return "ColonColon";
        case TokenType::Semicolon:
            return "Semicolon";
        case TokenType::LessThan:
            return "LessThan";
        case TokenType::LessThanEqual:
            return "LessThanEqual";
        case TokenType::LessThanLessThan:
            return "LessThanLessThan";
        case TokenType::LessThanLessThanEqual:
            return "LessThanLessThanEqual";
        case TokenType::Equal:
            return "Equal";
        case TokenType::EqualEqual:
            return "EqualEqual";
        case TokenType::GreaterThan:
            return "GreaterThan";
        case TokenType::GreaterThanEqual:
            return "GreaterThanEqual";
        case TokenType::GreaterThanGreaterThan:
            return "GreaterThanGreaterThan";
        case TokenType::GreaterThanGreaterThanEqual:
            return "GreaterThanGreaterThanEqual";
        case TokenType::QuestionMark:
            return "QuestionMark";
        case TokenType::AtSign:
            return "AtSign";
        case TokenType::Symbol:
            return "Symbol";
        case TokenType::LeftBracket:
            return "LeftBracket";
        case TokenType::Backslash:
            return "Backslash";
        case TokenType::RightBracket:
            return "RightBracket";
        case TokenType::Caret:
            return "Caret";
        case TokenType::Underscore:
            return "Underscore";
        case TokenType::Backtick:
            return "Backtick";
        case TokenType::LeftBrace:
            return "LeftBrace";
        case TokenType::VerticalBar:
            return "VerticalBar";
        case TokenType::RightBrace:
            return "RightBrace";
        case TokenType::Tilde:
            return "Tilde";
        }
        return "Unknown";
    }

    std::string Token::ToString() const
    {
        std::stringstream stringStream;
        stringStream << TypeString(Type);
        stringStream << " at index " << Index << ": '" << Text << "'";
        return stringStream.str();
    }
} // namespace Nebula::Shader
