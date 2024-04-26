cmake_minimum_required(VERSION 3.27)

add_subdirectory(Common) # Common code shared between all projects
add_subdirectory(Compiler)
add_subdirectory(Conductor)
add_subdirectory(CLITool)
#add_subdirectory(Builder) # Generates and builds shaders for the different platforms
