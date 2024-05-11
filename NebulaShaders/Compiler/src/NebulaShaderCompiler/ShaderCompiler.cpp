#include "ShaderCompiler.hpp"
#include <sstream>

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
                token = lexer.NextToken();
                COMPILER_EXPECT(LeftBrace, token);

                token = lexer.NextToken();
                while (token.Type != TokenType::RightBrace && token.Type != TokenType::EndOfFile)
                {
                    switch (token.Type)
                    {
                    case TokenType::UniformsKeyword: {
                        auto result = ParseUniformBuffer(lexer, token);
                        if (result.has_value())
                        {
                            return result;
                        }
                        break;
                    }

                    case TokenType::TexturesKeyword: {
                        auto result = ParseTexture(lexer, token);
                        if (result.has_value())
                        {
                            return result;
                        }
                        break;
                    }

                    default:
                        std::stringstream sstream;
                        sstream << "Unknown token inside scope 'Bindings': " << Token::TypeString(token.Type);
                        return Error {sstream.str(), token.Index};
                    }

                    token = lexer.NextToken();
                }

                if (token.Type == TokenType::EndOfFile)
                {
                    return Error {"Expected '}'", token.Index};
                }

                break;
            }
            case TokenType::InputKeyword:
            case TokenType::PixelInputKeyword: {
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
                std::stringstream sstream;
                sstream << "Unknown token: " << Token::TypeString(token.Type);
                return Error {sstream.str(), token.Index};
            }
        }

        return std::nullopt;
    }

    std::optional<Compiler::Error> Compiler::ParseUniformBuffer(Lexer& lexer, Token& token)
    {
        token = lexer.NextToken();
        COMPILER_EXPECT(StringLiteral, token);
        std::string bufferName = std::string(token.Text);

        token = lexer.NextToken();
        COMPILER_EXPECT(LeftBrace, token);

        auto scope = LexScope(lexer);

        if (scope.empty())
        {
            return Error {"Expected scope", token.Index};
        }

        COMPILER_EXPECT(RightBrace, scope.back());
        scope.pop_back();

        std::size_t index = 0;
        std::vector<Binding> bindings;

        while (index < scope.size())
        {
            // Type Name : Binding ;
            Binding binding;

            token = scope[index++];
            COMPILER_EXPECT(Symbol, token);
            binding.Type = token.Text;

            if (index >= scope.size())
            {
                return Error {"Expected name", token.Index};
            }
            token = scope[index++];
            COMPILER_EXPECT(Symbol, token);
            binding.Name = token.Text;

            if (index >= scope.size())
            {
                return Error {"Expected colon", token.Index};
            }
            token = scope[index++];
            COMPILER_EXPECT(Colon, token);

            if (index >= scope.size())
            {
                return Error {"Expected binding", token.Index};
            }
            token = scope[index++];
            COMPILER_EXPECT(Symbol, token);
            binding.Binding = token.Text;

            if (index >= scope.size())
            {
                return Error {"Expected semicolon", token.Index};
            }
            token = scope[index++];
            COMPILER_EXPECT(Semicolon, token);

            bindings.push_back(std::move(binding));
        }

        m_Meta.UniformBuffers.push_back({bufferName, std::move(bindings)});
        return std::nullopt;
    }

    std::optional<Compiler::Error> Compiler::ParseTexture(Lexer& lexer, Token& token)
    {
        token = lexer.NextToken();
        COMPILER_EXPECT(LeftBrace, token);

        auto scope = LexScope(lexer);

        if (scope.empty())
        {
            return Error {"Expected scope", token.Index};
        }

        COMPILER_EXPECT(RightBrace, scope.back());
        scope.pop_back();

        std::size_t index = 0;

        while (index < scope.size())
        {
            // Type Name : Binding ;
            Binding binding;

            token = scope[index++];
            COMPILER_EXPECT(Symbol, token);
            binding.Type = token.Text;

            if (index >= scope.size())
            {
                return Error {"Expected name", token.Index};
            }
            token = scope[index++];
            COMPILER_EXPECT(Symbol, token);
            binding.Name = token.Text;

            if (index >= scope.size())
            {
                return Error {"Expected colon", token.Index};
            }
            token = scope[index++];
            COMPILER_EXPECT(Colon, token);

            if (index >= scope.size())
            {
                return Error {"Expected binding", token.Index};
            }
            token = scope[index++];
            COMPILER_EXPECT(Symbol, token);
            binding.Binding = token.Text;

            if (index >= scope.size())
            {
                return Error {"Expected semicolon", token.Index};
            }
            token = scope[index++];
            COMPILER_EXPECT(Semicolon, token);

            m_Meta.Textures.push_back(std::move(binding));
        }

        return std::nullopt;
    }

    std::vector<SUniformBuffer> Compiler::GetUniformBuffers() const
    {
        // We map the vector of bindings to a vector of StringBindings
        std::vector<SUniformBuffer> uniformBuffers;

        uniformBuffers.reserve(m_Meta.UniformBuffers.size());
        for (const auto& buffer : m_Meta.UniformBuffers)
        {
            std::vector<StringBinding> bindings;
            bindings.reserve(buffer.Bindings.size());
            for (const auto& binding : buffer.Bindings)
            {
                bindings.emplace_back(binding.Type, binding.Name, binding.Binding);
            }

            uniformBuffers.emplace_back(buffer.Name, std::move(bindings));
        }

        return uniformBuffers;
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

#undef COMPILER_EXPECT
