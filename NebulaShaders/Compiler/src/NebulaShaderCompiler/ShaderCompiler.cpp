#include "ShaderCompiler.hpp"

#include "Lexer.hpp"

namespace Nebula::Shader
{
    static std::vector<Token> LexScope(Lexer& lexer)
    {
        std::vector<Token> tokens;
        std::size_t nesting = 0;
        for (auto token = lexer.NextToken(); token.Type != TokenType::EndOfFile; token = lexer.NextToken())
        {
            switch (token.Type)
            {
            case TokenType::LeftBrace:
                ++nesting;
                tokens.push_back(token);
                break;
            case TokenType::RightBrace:
                tokens.push_back(token);
                if (nesting == 0)
                {
                    return tokens;
                }
                --nesting;
                break;
            case TokenType::EndOfFile:
                tokens.emplace_back(token);
                return tokens;
            default:
                tokens.push_back(token);
                break;
            }
        }

        return tokens;
    }

    std::optional<Compiler::Error> Compiler::Compile()
    {
        Lexer lexer(m_Source);
        for (auto token = lexer.NextToken(); token.Type != TokenType::EndOfFile; token = lexer.NextToken())
        {
            switch (token.Type)
            {
            case TokenType::Comment:
            case TokenType::BlockComment:
                break;
            case TokenType::Invalid:
            case TokenType::InvalidComment:
            case TokenType::InvalidNumberLiteral:
            case TokenType::InvalidStringLiteral:
                return Error {"Invalid token", token.Index};
            case TokenType::NameKeyword:
                token = lexer.NextToken();
                if (token.Type != TokenType::StringLiteral)
                {
                    return Error {"Expected string literal", token.Index};
                }

                if (m_Meta.Name.empty())
                {
                    m_Meta.Name = token.Text;
                }
                else
                {
                    return Error {"Name already set", token.Index};
                }

                token = lexer.NextToken();
                if (token.Type != TokenType::Semicolon)
                {
                    return Error {"Expected semicolon", token.Index};
                }
                break;
            case TokenType::BindingsKeyword: {
                token = lexer.NextToken();
                if (token.Type != TokenType::LeftBrace)
                {
                    return Error {"Expected left brace", token.Index};
                }

                auto bindings = LexScope(lexer);
                if (bindings.empty())
                {
                    return Error {"Expected bindings", token.Index};
                }

                if (bindings.back().Type != TokenType::RightBrace)
                {
                    return Error {"Expected right brace", bindings.back().Index};
                }

                bindings.pop_back();

                for (std::size_t i = 0; i < bindings.size(); i++)
                {
                    if (bindings[i].Type == TokenType::UniformsKeyword ||
                        bindings[i].Type == TokenType::TexturesKeyword)
                    {
                        TokenType type = bindings[i].Type;

                        // Left brace
                        if (bindings[++i].Type != TokenType::LeftBrace)
                        {
                            return Error {"Expected left brace", bindings[i].Index};
                        }

                        while (bindings[++i].Type != TokenType::RightBrace && i < bindings.size())
                        {
                            // Type - Name - : -  Binding - Semicolon
                            Binding binding;
                            auto token = bindings[i];
                            if (token.Type != TokenType::Symbol)
                            {
                                return Error {"Expected a symbol", token.Index};
                            }
                            binding.Type = token.Text;

                            token = bindings[++i];
                            if (token.Type != TokenType::Symbol)
                            {
                                return Error {"Expected a symbol", token.Index};
                            }

                            binding.Name = token.Text;

                            token = bindings[++i];
                            if (token.Type != TokenType::Colon)
                            {
                                return Error {"Expected colon", token.Index};
                            }

                            token = bindings[++i];
                            if (token.Type != TokenType::Symbol)
                            {
                                return Error {"Expected a symbol", token.Index};
                            }

                            binding.Binding = token.Text;

                            token = bindings[++i];
                            if (token.Type != TokenType::Semicolon)
                            {
                                return Error {"Expected semicolon", token.Index};
                            }

                            if (type == TokenType::UniformsKeyword)
                            {
                                m_Meta.Uniforms.push_back(binding);
                            }
                            else
                            {
                                m_Meta.Textures.push_back(binding);
                            }
                        }

                        if (i == bindings.size())
                        {
                            return Error {"Expected right brace", bindings[i - 1].Index};
                        }
                    }
                }
                break;
            }
            case TokenType::InputKeyword:
            case TokenType::FragmentInputKeyword: {
                token = lexer.NextToken();
                if (token.Type != TokenType::LeftBrace)
                {
                    return Error {"Expected left brace", token.Index};
                }

                auto inputs = LexScope(lexer);
                if (inputs.empty())
                {
                    return Error {"Expected inputs", token.Index};
                }

                if (inputs.back().Type != TokenType::RightBrace)
                {
                    return Error {"Expected right brace", inputs.back().Index};
                }

                inputs.pop_back();

                for (std::size_t i = 0; i < inputs.size(); i++)
                {
                    if (inputs[i].Type == TokenType::InputKeyword)
                    {
                        TokenType type = inputs[i].Type;

                        // Left brace
                        if (inputs[++i].Type != TokenType::LeftBrace)
                        {
                            return Error {"Expected left brace", inputs[i].Index};
                        }

                        while (inputs[++i].Type != TokenType::RightBrace && i < inputs.size())
                        {
                            // Type - Name - : -  Binding - Semicolon
                            Binding binding;
                            auto token = inputs[i];
                            if (token.Type != TokenType::Symbol)
                            {
                                return Error {"Expected a symbol", token.Index};
                            }
                            binding.Type = token.Text;

                            token = inputs[++i];
                            if (token.Type != TokenType::Symbol)
                            {
                                return Error {"Expected a symbol", token.Index};
                            }

                            binding.Name = token.Text;

                            token = inputs[++i];
                            if (token.Type != TokenType::Colon)
                            {
                                return Error {"Expected colon", token.Index};
                            }

                            token = inputs[++i];
                            if (token.Type != TokenType::Symbol)
                            {
                                return Error {"Expected a symbol", token.Index};
                            }

                            binding.Binding = token.Text;

                            token = inputs[++i];
                            if (token.Type != TokenType::Semicolon)
                            {
                                return Error {"Expected semicolon", token.Index};
                            }

                            if (type == TokenType::InputKeyword)
                            {
                                m_Meta.Inputs.push_back(binding);
                            }
                            else
                            {
                                m_Meta.FragmentInputs.push_back(binding);
                            }
                        }

                        if (i == inputs.size())
                        {
                            return Error {"Expected right brace", inputs[i - 1].Index};
                        }
                    }
                }

                break;
            }
            case TokenType::ShaderKeyword: {
                token = lexer.NextToken();
                if (token.Type != TokenType::LeftBrace)
                {
                    return Error {"Expected left brace", token.Index};
                }

                for (token = lexer.NextToken();
                     token.Type != TokenType::RightBrace && token.Type != TokenType::EndOfFile;
                     token = lexer.NextToken())
                {
                    if (token.Type == TokenType::SharedKeyword)
                    {
                        if (m_Sources.Shared.has_value())
                        {
                            return Error {"Shared shader already set", token.Index};
                        }

                        // Left brace
                        if (lexer.NextToken().Type != TokenType::LeftBrace)
                        {
                            return Error {"Expected left brace", token.Index};
                        }

                        // Bit of a hack
                        auto toks = LexScope(lexer);

                        if (toks.empty())
                        {
                            return Error {"Expected source", token.Index};
                        }

                        if (toks.back().Type != TokenType::RightBrace)
                        {
                            return Error {"Expected right brace", toks.back().Index};
                        }

                        // Substring from first to last token
                        std::string source =
                            m_Source.substr(toks.front().Index, toks.back().Index - toks.front().Index);
                        toks.pop_back();

                        m_Sources.Shared = source;
                    }
                    else if (token.Type == TokenType::VertexKeyword)
                    {
                        if (m_Sources.Vertex.has_value())
                        {
                            return Error {"Vertex shader already set", token.Index};
                        }

                        // Left brace
                        if (lexer.NextToken().Type != TokenType::LeftBrace)
                        {
                            return Error {"Expected left brace", token.Index};
                        }

                        auto toks = LexScope(lexer);

                        if (toks.empty())
                        {
                            return Error {"Expected source", token.Index};
                        }

                        if (toks.back().Type != TokenType::RightBrace)
                        {
                            return Error {"Expected right brace", toks.back().Index};
                        }

                        // Substring from first to last token
                        std::string source =
                            m_Source.substr(toks.front().Index, toks.back().Index - toks.front().Index);
                        toks.pop_back();

                        m_Sources.Vertex = source;
                    }
                    else if (token.Type == TokenType::FragmentKeyword)
                    {
                        if (m_Sources.Fragment.has_value())
                        {
                            return Error {"Fragment shader already set", token.Index};
                        }

                        // Left brace
                        if (lexer.NextToken().Type != TokenType::LeftBrace)
                        {
                            return Error {"Expected left brace", token.Index};
                        }

                        auto toks = LexScope(lexer);

                        if (toks.empty())
                        {
                            return Error {"Expected source", token.Index};
                        }

                        if (toks.back().Type != TokenType::RightBrace)
                        {
                            return Error {"Expected right brace", toks.back().Index};
                        }

                        // Substring from first to last token
                        std::string source =
                            m_Source.substr(toks.front().Index, toks.back().Index - toks.front().Index);
                        toks.pop_back();

                        m_Sources.Fragment = source;
                    }
                    else if (token.Type == TokenType::EntrypointKeyword)
                    {
                        token = lexer.NextToken();
                        if (token.Type != TokenType::LeftBrace)
                        {
                            return Error {"Expected left brace", token.Index};
                        }

                        auto entrypoints = LexScope(lexer);
                        if (entrypoints.empty())
                        {
                            return Error {"Expected entrypoints", token.Index};
                        }

                        if (entrypoints.back().Type != TokenType::RightBrace)
                        {
                            return Error {"Expected right brace", entrypoints.back().Index};
                        }

                        entrypoints.pop_back();

                        for (std::size_t i = 0; i < entrypoints.size(); i++)
                        {
                            if (entrypoints[i].Type == TokenType::VertexKeyword)
                            {
                                auto token = entrypoints[++i];
                                if (token.Type != TokenType::StringLiteral)
                                {
                                    return Error {"Expected string literal", token.Index};
                                }

                                if (!m_Meta.VertexEntrypoint.empty())
                                {
                                    return Error {"Vertex entrypoint already set", token.Index};
                                }
                                m_Meta.VertexEntrypoint = token.Text;

                                token = entrypoints[++i];
                                if (token.Type != TokenType::Semicolon)
                                {
                                    return Error {"Expected semicolon", token.Index};
                                }
                            }
                            else if (entrypoints[i].Type == TokenType::FragmentKeyword)
                            {
                                auto token = entrypoints[++i];
                                if (token.Type != TokenType::StringLiteral)
                                {
                                    return Error {"Expected string literal", token.Index};
                                }

                                if (!m_Meta.FragmentEntrypoint.empty())
                                {
                                    return Error {"Fragment entrypoint already set", token.Index};
                                }
                                m_Meta.FragmentEntrypoint = token.Text;

                                token = entrypoints[++i];
                                if (token.Type != TokenType::Semicolon)
                                {
                                    return Error {"Expected semicolon", token.Index};
                                }
                            }
                        }
                    }
                    else
                    {
                        return Error {"Unknown token inside scope 'Shader'", token.Index};
                    }
                }
                if (token.Type != TokenType::RightBrace)
                {
                    return Error {"Expected right brace", token.Index};
                }
            }
            default:
                break;
            }
        }

        return std::nullopt;
    }

    std::array<std::size_t, 2> Compiler::GetRowColumn(std::size_t index) const
    {
        std::size_t row    = 1;
        std::size_t column = 1;
        for (std::size_t i = 0; i < index; i++)
        {
            if (m_Source[i] == '\n')
            {
                ++row;
                column = 1;
            }
            else
            {
                ++column;
            }
        }

        return {row, column};
    }
} // namespace Nebula::Shader
