#include <fuzztest/fuzztest.h>
#include <gtest/gtest.h>

#include "NebulaShaderCompiler/Lexer.hpp"

TEST(LexerFuzzTests, LexerInitializes) { Nebula::Shader::Lexer lexer(""); }

void LexerNeverCrashes(const std::string& input)
{
    Nebula::Shader::Lexer lexer(input);
    while (lexer.NextToken().Type != Nebula::Shader::TokenType::EndOfFile)
    {
    }
}

FUZZTEST(LexerFuzzTests, LexerNeverCrashes).WithDomains(fuzztest::PrintableString());
