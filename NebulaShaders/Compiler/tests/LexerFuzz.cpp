#include <fuzztest/fuzztest.h>
#include <gtest/gtest.h>

#include "NebulaShaderCompiler/Lexer.hpp"

void LexerNeverCrashes(const std::string& input) {
    Nebula::Shader::Lexer lexer(input);
  while (lexer.NextToken().Type != Nebula::Shader::TokenType::EndOfFile) {
  }
}

FUZZTEST(LexerFuzzTests, LexerNeverCrashes)
    .WithDomains(fuzztest::PrintableString());
