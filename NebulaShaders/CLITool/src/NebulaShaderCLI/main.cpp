#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "NebulaCore/Log.hpp"
#include "NebulaShaderCompiler/Lexer.hpp"
#include "NebulaShaderCompiler/Token.hpp"

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        args.emplace_back(argv[i]);
    }

    if (args.size() < 2)
    {
        std::cerr << "Usage: " << args[0] << " <shader file>\n";
        return 1;
    }

    std::ifstream file(args[1]);
    if (!file.good())
    {
        std::cerr << "Failed to read file: " << args[1] << '\n';
        return 1;
    }

    // Read the file
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string src = buffer.str();
    Nebula::Shader::Lexer lexer(src);

    Nebula::Token token;
    do
    {
        token = lexer.NextToken();
        std::cout << token.ToString() << '\n';
    } while (token.Type != Nebula::TokenType::EndOfFile);

    return 0;
}
