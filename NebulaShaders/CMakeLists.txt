cmake_minimum_required(VERSION 3.27)

add_subdirectory(Common) # Common code shared between all projects
add_subdirectory(Compiler)
add_subdirectory(Conductor)
add_subdirectory(CLITool)
add_subdirectory(Builder) # Generates and builds shaders for the different platforms

add_library(NebulaShaders INTERFACE)

if (NEBULA_ENABLE_CLANG_FORMAT)
    add_custom_target(NebulaShaders_format)
    add_dependencies(NebulaShaders_format
        NebulaShaderBuilder_format
        NebulaShaderCLI_format
        NebulaShaderCompiler_format
        NebulaShaderCommon_format
        NebulaShaderConductor_format
    )
endif()
