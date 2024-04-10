#include "Parser.hpp"

namespace Nebula::Shader
{


    // =================================================================================================
    // Parser BacktrackableLexer
    // =================================================================================================

    Token Parser::BacktrackableLexer::Peek(std::size_t offset)
    {
        if (CurrentTokenIndex + offset >= EndOfStreamIndex)
        {
            return {"", EndOfStreamIndex, TokenType::EndOfFile};
        }

        // For the first token 0 + 0 = 0
        if (CurrentTokenIndex + offset >= TokensRead.size())
        {
            ReadTokens(CurrentTokenIndex + offset - TokensRead.size() + 1);
        }

        return TokensRead[CurrentTokenIndex + offset];
    }

    Token Parser::BacktrackableLexer::Read()
    {
        if (CurrentTokenIndex >= EndOfStreamIndex)
        {
            return {"", EndOfStreamIndex, TokenType::EndOfFile};
        }

        Token token = Peek();
        CurrentTokenIndex++;
        return token;
    }

    void Parser::BacktrackableLexer::Consume(std::size_t count)
    {
        if (CurrentTokenIndex + count >= EndOfStreamIndex)
        {
            return;
        }

        CurrentTokenIndex += count;
    }

    bool Parser::BacktrackableLexer::Consume(TokenType type)
    {
        if (Peek().Type == type)
        {
            Consume();
            return true;
        }

        return false;
    }

    bool Parser::BacktrackableLexer::Consume(TokenType type, Token& token)
    {
        if (Peek().Type == type)
        {
            token = Read();
            return true;
        }

        return false;
    }

    void Parser::BacktrackableLexer::Backtrack(std::size_t count)
    {
        if (CurrentTokenIndex < count)
        {
            // We try to be tolerant, so we just backtrack to the start
            CurrentTokenIndex = 0;
            return;
        }

        CurrentTokenIndex -= count;
    }

    void Parser::BacktrackableLexer::BacktrackTo(std::size_t index)
    {
        if (index >= EndOfStreamIndex)
        {
            return;
        }

        CurrentTokenIndex = index;
    }

    void Parser::BacktrackableLexer::GoTo(std::size_t index)
    {
        if (index >= EndOfStreamIndex)
        {
            return;
        }

        CurrentTokenIndex = index;
    }

    void Parser::BacktrackableLexer::ReadTokens(std::size_t count)
    {
        for (std::size_t i = 0; i < count;)
        {
            Token token = m_Lexer.NextToken();
            switch (token.Type)
            {
            case TokenType::Comment:
            case TokenType::BlockComment:
                break; // Ignore comments
            default:
                TokensRead.push_back(token);
                i++;
                break;
            }
        }
    }

} // namespace Nebula::Shader
