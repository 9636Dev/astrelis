#include "ShaderCompiler.hpp"

#define COMPILER_EXPECT(tokType, tokRef)                                                                   \
    if (tokRef.Type != TokenType::tokType)                                                                 \
    {                                                                                                      \
        return Error {"Expected token " #tokType " got: " + Token::TypeString(tokRef.Type), tokRef.Index}; \
    }

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

    std::optional<Compiler::Error> Compiler::Compile() // NOLINT(readability-function-cognitive-complexity)
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
                COMPILER_EXPECT(StringLiteral, token);

                if (!m_Meta.Name.empty())
                {
                    return Error {"Name already set", token.Index};
                }

                m_Meta.Name = token.Text;
                token       = lexer.NextToken();
                COMPILER_EXPECT(Semicolon, token);
                break;
            case TokenType::BindingsKeyword: {
                auto res = ParseBindings(lexer, token);
                if (res.has_value())
                {
                    return res;
                }
                break;
            }
            case TokenType::InputKeyword:
            case TokenType::FragmentInputKeyword: {
                auto& inputsVec = token.Type == TokenType::InputKeyword ? m_Meta.Inputs : m_Meta.FragmentInputs;
                token           = lexer.NextToken();
                COMPILER_EXPECT(LeftBrace, token);

                auto inputs = LexScope(lexer);
                if (inputs.empty())
                {
                    return Error {"Expected inputs", token.Index};
                }

                COMPILER_EXPECT(RightBrace, inputs.back());
                inputs.pop_back();

                std::size_t index = 0;
                while (index + 4 < inputs.size())
                {
                    // Type - Name - : -  Binding - Semicolon
                    Binding binding;
                    auto token = inputs[index];
                    COMPILER_EXPECT(Symbol, token);
                    binding.Type = token.Text;

                    token = inputs[++index];
                    COMPILER_EXPECT(Symbol, token);
                    binding.Name = token.Text;

                    token = inputs[++index];
                    COMPILER_EXPECT(Colon, token);

                    token = inputs[++index];
                    COMPILER_EXPECT(Symbol, token);
                    binding.Binding = token.Text;

                    token = inputs[++index];
                    COMPILER_EXPECT(Semicolon, token);

                    inputsVec.push_back(std::move(binding));
                    index++;
                }

                if (index != inputs.size())
                {
                    return Error {"Invalid Syntax for Input", inputs[index].Index};
                }

                break;
            }
            case TokenType::ShaderKeyword: {
                token = lexer.NextToken();
                COMPILER_EXPECT(LeftBrace, token);

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

                        token = lexer.NextToken();
                        COMPILER_EXPECT(LeftBrace, token);

                        // Bit of a hack
                        auto toks = LexScope(lexer);

                        if (toks.empty())
                        {
                            return Error {"Expected source", token.Index};
                        }

                        COMPILER_EXPECT(RightBrace, toks.back());

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
                        token = lexer.NextToken();
                        COMPILER_EXPECT(LeftBrace, token);

                        auto toks = LexScope(lexer);

                        if (toks.empty())
                        {
                            return Error {"Expected source", token.Index};
                        }

                        COMPILER_EXPECT(RightBrace, toks.back());

                        // Substring from first to last token
                        std::string source =
                            m_Source.substr(toks.front().Index, toks.back().Index - toks.front().Index);
                        toks.pop_back();

                        m_Sources.Vertex = source;
                    }
                    else if (token.Type == TokenType::FragmentKeyword)
                    {
                        if (m_Sources.Pixel.has_value())
                        {
                            return Error {"Fragment shader already set", token.Index};
                        }

                        token = lexer.NextToken();
                        COMPILER_EXPECT(LeftBrace, token);

                        auto toks = LexScope(lexer);

                        if (toks.empty())
                        {
                            return Error {"Expected source", token.Index};
                        }

                        COMPILER_EXPECT(RightBrace, toks.back());

                        // Substring from first to last token
                        std::string source =
                            m_Source.substr(toks.front().Index, toks.back().Index - toks.front().Index);
                        toks.pop_back();

                        m_Sources.Pixel = source;
                    }
                    else if (token.Type == TokenType::EntrypointKeyword)
                    {
                        token = lexer.NextToken();
                        COMPILER_EXPECT(LeftBrace, token);

                        auto entrypoints = LexScope(lexer);
                        if (entrypoints.empty())
                        {
                            return Error {"Expected entrypoints", token.Index};
                        }

                        COMPILER_EXPECT(RightBrace, entrypoints.back());

                        entrypoints.pop_back();

                        for (std::size_t i = 0; i < entrypoints.size(); i++)
                        {
                            if (entrypoints[i].Type == TokenType::VertexKeyword)
                            {
                                auto token = entrypoints[++i];
                                COMPILER_EXPECT(StringLiteral, token);

                                if (!m_Meta.VertexEntrypoint.empty())
                                {
                                    return Error {"Vertex entrypoint already set", token.Index};
                                }
                                m_Meta.VertexEntrypoint = token.Text;

                                token = entrypoints[++i];
                                COMPILER_EXPECT(Semicolon, token);
                            }
                            else if (entrypoints[i].Type == TokenType::FragmentKeyword)
                            {
                                auto token = entrypoints[++i];
                                COMPILER_EXPECT(StringLiteral, token);

                                if (!m_Meta.FragmentEntrypoint.empty())
                                {
                                    return Error {"Fragment entrypoint already set", token.Index};
                                }
                                m_Meta.FragmentEntrypoint = token.Text;

                                token = entrypoints[++i];
                                COMPILER_EXPECT(Semicolon, token);
                            }
                        }
                    }
                    else
                    {
                        return Error {"Unknown token inside scope 'Shader'", token.Index};
                    }
                }
                COMPILER_EXPECT(RightBrace, token);
            }
            case TokenType::EndOfFile:
                break;
            default:
                return Error {"Unknown token", token.Index};
            }
        }

        return std::nullopt;
    }

    std::optional<Compiler::Error>
        Compiler::ParseBindings(Lexer& lexer, Token& token) // NOLINT(readability-function-cognitive-complexity)
    {
        token = lexer.NextToken();
        COMPILER_EXPECT(LeftBrace, token);

        auto bindings = LexScope(lexer);
        if (bindings.empty())
        {
            return Error {"Expected bindings", token.Index};
        }

        COMPILER_EXPECT(RightBrace, bindings.back());
        bindings.pop_back();

        for (std::size_t i = 0; i < bindings.size(); i++)
        {
            if (bindings[i].Type == TokenType::UniformsKeyword || bindings[i].Type == TokenType::TexturesKeyword)
            {
                auto& bindingsVec =
                    bindings[i++].Type == TokenType::UniformsKeyword ? m_Meta.Uniforms : m_Meta.Textures;
                COMPILER_EXPECT(LeftBrace, bindings[i]);

                while (bindings[++i].Type != TokenType::RightBrace && i < bindings.size())
                {
                    // Type Name : Binding ;
                    Binding binding;

                    auto token = bindings[i];
                    COMPILER_EXPECT(Symbol, token);
                    binding.Type = token.Text;

                    token = bindings[++i];
                    COMPILER_EXPECT(Symbol, token);
                    binding.Name = token.Text;

                    token = bindings[++i];
                    COMPILER_EXPECT(Colon, token);

                    token = bindings[++i];
                    COMPILER_EXPECT(Symbol, token);
                    binding.Binding = token.Text;

                    token = bindings[++i];
                    COMPILER_EXPECT(Semicolon, token);

                    bindingsVec.push_back(binding);
                }

                if (i == bindings.size())
                {
                    return Error {"Expected closing brace for Bindings", bindings[i].Index};
                }
            }
        }

        return std::nullopt;
    }

    std::vector<StringBinding> Compiler::GetBindings() const
    {
        // We map the vector of bindings to a vector of StringBindings
        std::vector<StringBinding> bindings;
        bindings.reserve(m_Meta.Uniforms.size());
        for (const auto& binding : m_Meta.Uniforms)
        {
            bindings.emplace_back(binding.Type, binding.Name, binding.Binding);
        }

        return bindings;
    }

    std::vector<StringBinding> Compiler::GetTextures() const
    {
        std::vector<StringBinding> textures;
        textures.reserve(m_Meta.Textures.size());
        for (const auto& binding : m_Meta.Textures)
        {
            textures.emplace_back(binding.Type, binding.Name, binding.Binding);
        }

        return textures;
    }

    std::vector<StringInput> Compiler::GetInputs() const
    {
        std::vector<StringInput> inputs;
        inputs.reserve(m_Meta.Inputs.size());
        for (const auto& input : m_Meta.Inputs)
        {
            inputs.emplace_back(input.Type, input.Name, input.Binding);
        }

        return inputs;
    }

    std::vector<StringInput> Compiler::GetPixelInputs() const
    {
        std::vector<StringInput> inputs;
        inputs.reserve(m_Meta.FragmentInputs.size());
        for (const auto& input : m_Meta.FragmentInputs)
        {
            inputs.emplace_back(input.Type, input.Name, input.Binding);
        }

        return inputs;
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

#undef COMPILER_EXPECT_INTERNAL
#undef COMPILER_EXPECT
