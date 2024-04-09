#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "NebulaShaderCompiler/Lexer.hpp"
#include "NebulaShaderCompiler/Token.hpp"

int main(int argc, char** argv)
{
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

    std::ofstream file(args[1]);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << args[1] << '\n';
        return 1;
    }

    // Read the file
    std::ostringstream sstr;
    sstr << file.rdbuf();
    std::string src = sstr.str();
    Nebula::Shader::Lexer lexer(src);

    Nebula::Token token;
    do
    {
        token = lexer.NextToken();
        std::cout << token.ToString() << '\n';
    } while (token.Type != Nebula::TokenType::EndOfFile);

    return 0;
}
