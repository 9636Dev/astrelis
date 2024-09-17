#include "Astrelis/IO/File.hpp"

#include <ShaderCompiler/ShaderConductor.hpp>
#include <fstream>
#include <iostream>

#include "ShaderCompiler/ShaderCompiler.hpp"

using Astrelis::File;
using AstrelisEditor::ShaderConductor, Astrelis::ShaderStage;

struct Source {
    File        file;
    std::string entrypoint;
    ShaderStage stage;
    File        output;
};

constexpr std::uint32_t kVersionMajor = 1;
constexpr std::uint32_t kVersionMinor = 0;

int main(int argc, char** argv) {
    try {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        std::vector<std::string> args(argv, argv + argc);
        if (args.size() < 2) {
            std::cerr << "Provide --debug or --release" << std::endl;
            return 1;
        }
        std::cout << "Compiling shaders: " << args[0] << " " << args[1] << std::endl;
        ShaderConductor conductor;
        if (conductor.Init().IsErr()) {
            throw std::runtime_error("Failed to initialize ShaderCompiler");
        }
        ShaderConductor::HLSLOptions options;
        if (std::string(args[1]) == "--debug") {
            options.OptimizationLevel = 0;
        }
        else if (std::string(args[1]) == "--release") {
            options.OptimizationLevel = 3;
        }
        else {
            std::cerr << "Provide --debug or --release" << std::endl;
            return 1;
        }

        AstrelisEditor::ShaderCompiler compiler;
        if (compiler.Init().IsErr()) {
            throw std::runtime_error("Failed to initialize ShaderCompiler");
        }

        auto shaderSource = File("resources/shaders/Basic.hlsl").ReadText();
        if (shaderSource.IsErr()) {
            throw std::runtime_error(shaderSource.UnwrapErr());
        }

        auto shaderRes = compiler.CompileShader({
            {
             .Source     = shaderSource.Unwrap(),
             .Stage      = ShaderStage::Vertex,
             .Entrypoint = "VS_Main",
             },
            {
             .Source     = shaderSource.Unwrap(),
             .Stage      = ShaderStage::Fragment,
             .Entrypoint = "PS_Main",
             },
        });

        if (shaderRes.IsErr()) {
            throw std::runtime_error(shaderRes.UnwrapErr());
        }

        auto shaderFormat = shaderRes.Unwrap();
        if (!File("resources/shaders/Basic.astshader").CanWrite()) {
            throw std::runtime_error("Failed to write to output file");
        }

        Astrelis::File file("resources/shaders/Basic.astshader");
        file.WriteBinaryStructure(shaderFormat).Expect("Failed to write to output file");

        compiler.Shutdown();
        conductor.Shutdown();

        std::cout << "Shaders compiled successfully" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
