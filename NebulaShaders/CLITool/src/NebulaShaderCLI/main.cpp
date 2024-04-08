#include <iostream>
#include <vector>

#include "NebulaShaderCompiler/Lexer.hpp"

int main(int argc, char** argv)
{
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        args.emplace_back(argv[i]);
    }

    // TODO: Parse args


    std::string src;
    Nebula::Shader::Lexer lexer(src);

    return 0;
}
