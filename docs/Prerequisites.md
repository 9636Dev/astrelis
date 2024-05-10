# Prerequisites for Nebula Engine

## Required Software
- CMake 3.27 or higher
- C++20 compatible compiler

### Windows

- Ninja or Visual Studio 2022
- DirectX SDK (Only required for DirectX12 backend)
- DirectXCompiler (Required to use ShaderConductor)

## Optional Software

- GLEW (Without GLEW, you need to specify the cmake flag: -DNEBULA_BUILD_GLEW=ON)
