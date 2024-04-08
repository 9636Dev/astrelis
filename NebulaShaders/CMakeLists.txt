cmake_minimum_required(VERSION 3.27)
project(NebulaShaders)

# Currently there is only the Shader Compiler, but we might add more tools in the future
add_subdirectory(Compiler)
add_subdirectory(CLITool)
