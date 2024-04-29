#include "NebulaIO/File.hpp"
#include "NebulaShaderBuilder/FileFormat.hpp"
#include "NebulaShaderConductor/ShaderOutput.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

enum class Output
{
    Glsl,
};

int CompileFile(Nebula::File& file, Output outputType, std::uint32_t outputVersion)
{
    std::stringstream sstream;
    std::ifstream ifs(file.GetPath());
    if (!ifs.good())
    {
        std::cerr << "Failed to open file " << file.GetPath() << std::endl;
        return 1;
    }

    std::cout << "Reading " << file.GetPath() << std::endl;

    sstream << ifs.rdbuf();
    ifs.close();

    std::cout << "Compiling " << file.GetPath() << std::endl;

    auto [output, err] = Nebula::Shader::IntermediateFormat::FromSource(sstream.str());
    if (!err.empty())
    {
        std::cerr << err << std::endl;
        return 1;
    }

    std::cout << "Compiled " << file.GetPath() << std::endl;

    auto outputFile = Nebula::File::FromPathString(file.GetParentPath() /
                                                   std::filesystem::path(file.GetFileNameWithoutExtension() + ".cnsl"));

    if (outputType == Output::Glsl)
    {
        Nebula::ShaderConductor::GLSLOutput glslOutput;
        glslOutput.Version = outputVersion;

        auto res = output.GenerateGlsl(glslOutput);
        if (!res.empty())
        {
            std::cerr << res << std::endl;
            return 1;
        }
    }

    auto res        = output.WriteToFile(outputFile);

    if (!res.Success)
    {
        std::cerr << "Failed to write output file" << std::endl;
        return 1;
    }

    std::cout << "Written " << res.BytesWritten << " bytes to " << outputFile.GetPath() << std::endl;

    return 0;
}

int main(int argc, char** argv)
{
    std::vector<std::string> args(argv,
                                       argv + argc); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

    // TODO(9636D): Support multiple outputs (Since our format technically supports it)
    if (args.size() != 4)
    {
        std::cerr << "Usage: " << args[0] << " <input> <output> <output-version>" << std::endl;
        return 1;
    }

    // Only first argument is used, the rest are ignored
    std::string input = args[1];
    std::string output = args[2];
    std::uint32_t outputVersion = std::stoul(args[3]);

    Output outputType = Output::Glsl;
    if (output == "glsl")
    {
        outputType = Output::Glsl;
    }
    else
    {
        std::cerr << "Unknown output type " << output << std::endl;
        return 1;
    }

    Nebula::File file(input);

    if (file.IsFile())
    {
        return CompileFile(file, outputType, outputVersion);
    }

    // Directory
    std::cerr << "Directory compilation not supported yet" << std::endl;
    return 1;
}
