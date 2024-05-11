#include "Lexer.hpp"

#include "NebulaCore/Assert.hpp"

#include <unordered_map>

namespace Nebula::Shader
{
    static std::unordered_map<std::string_view, TokenType> GetKeywordsMap()
    {
        return {
            {"Name",       TokenType::NameKeyword         },
            {"Bindings",   TokenType::BindingsKeyword     },
            {"Uniforms",   TokenType::UniformsKeyword     },
            {"Textures",   TokenType::TexturesKeyword     },
            {"Input",      TokenType::InputKeyword        },
            {"PixelInput", TokenType::PixelInputKeyword},
            {"Shader",     TokenType::ShaderKeyword       },
            {"Shared",     TokenType::SharedKeyword       },
            {"Vertex",     TokenType::VertexKeyword       },
            {"Fragment",   TokenType::FragmentKeyword     },
            {"Generated",  TokenType::GeneratedKeyword    },
            {"Entrypoint", TokenType::EntrypointKeyword   },
        };
    }

    Lexer::Lexer(std::string_view source) noexcept : m_Source(source) {}

    Lexer::Lexer(const Lexer&)            = default;
    Lexer& Lexer::operator=(const Lexer&) = default;

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    Token Lexer::NextToken() noexcept
    {
        if (m_Source.empty())
        {
            return {std::string(), 0, TokenType::EndOfFile};
        }
        static const auto keywords = GetKeywordsMap();
        SkipWhitespace();

        Token token;
        token.Index = m_Index;

        char character = CurrentChar();
        if (m_State == LexerState::InvalidAscii)
        {
            token.Type = TokenType::Invalid;
            Advance();
            token.Text = m_Source.substr(token.Index, m_Index - token.Index);
            m_State    = LexerState::NoError;
            return token;
        }

        if (m_State == LexerState::EndOfFile)
        {
            token.Type = TokenType::EndOfFile;
            return token;
        }

        switch (character)
        {
        case '\0':
            token.Type = TokenType::EndOfFile;
            break;
        case '!':
            if (PeekChar() == '=')
            {
                token.Type = TokenType::ExclamationMarkEqual;
                Advance();
                break;
            }
            token.Type = TokenType::ExclamationMark;
            break;
        case '"':
            return LexStringLiteral();
        case '#':
            token.Type = TokenType::Hash;
            break;
        case '$':
            token.Type = TokenType::Dollar;
            break;
        case '%':
            token.Type = TokenType::Percent;
            break;
        case '&':
            token.Type = TokenType::Ampersand;
            break;
        case '\'':
            token.Type = TokenType::Apostrophe;
            if (std::isspace(PeekChar()) == 0)
            {
                token.Type = TokenType::CharacterLiteral;
                Advance();
            }
            break;
        case '(':
            token.Type = TokenType::LeftParenthesis;
            break;
        case ')':
            token.Type = TokenType::RightParenthesis;
            break;
        case '*':
            token.Type = TokenType::Asterisk;
            break;
        case '+':
            token.Type = TokenType::Plus;
            break;
        case ',':
            token.Type = TokenType::Comma;
            break;
        case '-':
            token.Type = TokenType::Minus;
            break;
        case '.':
            token.Type = TokenType::Period;
            break;
        case '/':
            if (PeekChar() == '/')
            {
                return LexComment();
            }
            if (PeekChar() == '*')
            {
                return LexBlockComment();
            }
            token.Type = TokenType::Slash;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            token.Type = TokenType::NumberLiteral;
            break;
        case ':':
            if (PeekChar() == ':')
            {
                token.Type = TokenType::ColonColon;
                Advance();
            }
            else
            {
                token.Type = TokenType::Colon;
            }
            break;
        case ';':
            token.Type = TokenType::Semicolon;
            break;
        case '<':
            if (PeekChar() == '=')
            {
                token.Type = TokenType::LessThanEqual;
                Advance();
            }
            else if (PeekChar() == '<')
            {
                token.Type = TokenType::LessThanLessThan;
                Advance();
                if (PeekChar() == '=')
                {
                    token.Type = TokenType::LessThanLessThanEqual;
                    Advance();
                }
            }
            else
            {
                token.Type = TokenType::LessThan;
            }
            break;
        case '>':
            if (PeekChar() == '=')
            {
                token.Type = TokenType::GreaterThanEqual;
                Advance();
            }
            else if (PeekChar() == '>')
            {
                token.Type = TokenType::GreaterThanGreaterThan;
                Advance();
                if (PeekChar() == '=')
                {
                    token.Type = TokenType::GreaterThanGreaterThanEqual;
                    Advance();
                }
            }
            else
            {
                token.Type = TokenType::GreaterThan;
            }
            break;
        case '=':
            if (PeekChar() == '=')
            {
                token.Type = TokenType::EqualEqual;
                Advance();
            }
            else
            {
                token.Type = TokenType::Equal;
            }
            break;
        case '?':
            token.Type = TokenType::QuestionMark;
            break;
        case '@':
            token.Type = TokenType::AtSign;
            break;
        case '[':
            token.Type = TokenType::LeftBracket;
            break;
        case '\\':
            token.Type = TokenType::Backslash;
            break;
        case ']':
            token.Type = TokenType::RightBracket;
            break;
        case '^':
            token.Type = TokenType::Caret;
            break;
        case '_':
            token.Type = TokenType::Underscore;
            break;
        case '`':
            token.Type = TokenType::Backtick;
            break;
        case '{':
            token.Type = TokenType::LeftBrace;
            break;
        case '|':
            token.Type = TokenType::VerticalBar;
            break;
        case '}':
            token.Type = TokenType::RightBrace;
            break;
        case '~':
            token.Type = TokenType::Tilde;
            break;
        default:
            // Should only be alpha characters, but we assert just in case
            NEB_ASSERT(std::isalpha(CurrentChar()) != 0, "Invalid character in source");
            token = LexSymbol();
            // Match the symbol to a keyword
            if (keywords.find(token.Text) != keywords.end())
            {
                token.Type = keywords.at(token.Text);
            }
            return token;
        }

        Advance();
        token.Text = m_Source.substr(token.Index, m_Index - token.Index);
        return token;
    }

    Token Lexer::LexStringLiteral() noexcept
    {
        Advance(); // Skip the opening dqoute
        auto start = m_Index;

        while (true)
        {
            Advance();
            char current = CurrentChar();
            if (current == '\0' || current == '\n' || current == '\r')
            {
                return {m_Source.substr(start, m_Index - start), start, TokenType::InvalidStringLiteral};
            }
            if (current == '"')
            {
                break; // TODO(9636D): Handle escape characters
            }
        }
        Advance(); // Skip the closing quote

        // -1 because we advance one extra time
        return {m_Source.substr(start, m_Index - start - 1), start, TokenType::StringLiteral};
    }

    Token Lexer::LexNumberLiteral() noexcept
    {
        // TODO(9636D): Handle different number types, such as hex, binary, and octal
        auto start = m_Index;
        while (std::isdigit(CurrentChar()) != 0)
        {
            Advance();
        }

        if (std::isalpha(CurrentChar()) != 0)
        {
            return {m_Source.substr(start, m_Index - start), start, TokenType::InvalidNumberLiteral};
        }

        return {m_Source.substr(start, m_Index - start), start, TokenType::NumberLiteral};
    }

    Token Lexer::LexSymbol() noexcept
    {
        auto start     = m_Index;
        char character = CurrentChar();
        while (std::isalnum(character) != 0 || character == '_')
        {
            Advance();
            character = CurrentChar();
        }

        return {m_Source.substr(start, m_Index - start), start, TokenType::Symbol};
    }

    Token Lexer::LexComment() noexcept
    {
        auto start = m_Index;
        Advance(); // Skip the opening slash
        Advance(); // Skip the second slash

        char character = CurrentChar();
        while (character != '\n' && character != '\r' && m_State == LexerState::NoError)
        {
            Advance();
            character = CurrentChar();
        }

        return {m_Source.substr(start, m_Index - start), start, TokenType::Comment};
    }

    Token Lexer::LexBlockComment() noexcept
    {
        auto start = m_Index;
        Advance(); // Skip the opening slash
        Advance(); // Skip the star

        char character = CurrentChar();
        while (m_State == LexerState::NoError)
        {
            if (character == '*' && PeekChar() == '/')
            {
                Advance(); // Skip the star
                Advance(); // Skip the closing slash
                break;
            }
            Advance();
            character = CurrentChar();
        }

        // If we reached the end of the file, the block comment is invalid
        return {m_Source.substr(start, m_Index - start), start,
                m_State == LexerState::NoError ? TokenType::BlockComment : TokenType::InvalidComment};
    }

    // ============================
    // Text Processing Functions
    // ============================

    void Lexer::SkipWhitespace() noexcept
    {
        while (std::isspace(CurrentChar()) != 0 && m_State == LexerState::NoError)
        {
            Advance();
        }
    }

    char Lexer::CurrentChar() const noexcept { return CheckChar(m_Source[m_Index]); }

    char Lexer::PeekChar() const noexcept
    {
        if (m_Index + 1 >= m_Source.size())
        {
            m_State = LexerState::EndOfFile;
            return '\0';
        }
        return CheckChar(m_Source[m_Index + 1]);
    }

    char Lexer::CheckChar(char character) const noexcept
    {
        // NOLINTNEXTLINE
        if (!std::isprint(character) && !std::isspace(character))
        {
            m_State = LexerState::InvalidAscii;
            return '\0';
        }

        return character;
    }

    void Lexer::Advance() noexcept
    {
        if (m_Index + 1 >= m_Source.size())
        {
            m_State = LexerState::EndOfFile;
            return;
        }
        m_Index++;
    }

} // namespace Nebula::Shader
