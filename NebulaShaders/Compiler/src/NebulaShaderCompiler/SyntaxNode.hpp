#pragma once

#include "Token.hpp"

#include <optional>
#include <vector>

namespace Nebula::Shader
{
    enum class SyntaxNodeType
    {
        None, // Placeholder default value
        Identifier, // name
        TrailingIdentifier, // name.value
    };


    struct SyntaxNode
    {
        SyntaxNodeType Type = SyntaxNodeType::None;
        std::optional<Token> Value;
        std::vector<SyntaxNode> Children;

        SyntaxNode() = default;
        SyntaxNode(SyntaxNodeType type, Token token) : Type(type), Value(token) {}
    };
} // namespace Nebula::Shader
