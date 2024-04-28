#include "NebulaIO/File.hpp"
#include "NebulaShaderBuilder/Glsl.hpp"

int main(int argc, char** argv)
{
    std::vector<std::string> args(argv, argv + argc); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

    if (args.size() < 2)
    {
        std::cerr << "Usage: " << args[0] << " <input file / input dir>" << std::endl;
        return 1;
    }

    // Only first argument is used, the rest are ignored
    std::string input = args[1];
    Nebula::File file(input);
}
