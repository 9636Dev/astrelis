# Nebula Shader Format Design

**Specification and overview can be viewed here: ** [Nebula Shader Format](ShaderFormat.md)

## This focuses on the shader design details

### Overview

The shader design consists of the following components:
- **Stage 1**: Readable .nsl file
- **Stage 2**: Shader Generation (to Intermediate Representation)
- **Stage 2.1**: NebulaShaderFormat file Lexing and Parsing (represented in code)
- **Stage 2.2**: Shader generation (for platforms), using Metadata, storing platform specific code and/or metadata. **See section [Shader Generation](###Shader-Generation)**
- **Stage 3**: (Runtime stage) Shader loading / compiling (OpenGL), using metadata to bind correct semantics to shader variables.

### Shader Generation
This represented Stage 2.2 in the overview. Shader generating supports storing all the code / metadata in a single for different platforms.
The metadata (Names and types to binding targets) for shaders that are shared between platforms are stored in the intermediate file.

#### GLSL
GLSL code is stored in the intermediate file as a string. This is then used to generate the GLSL shader.
Alongside the glsl code, the glsl specific metadata is stored in the intermediate file, this includes:
- **Uniforms**: They are stored with their original name, mapped to glsl uniform name (ubo_NAME) and optionally the binding point.
- **Textures**: They are stored with their original name, mapped to glsl uniform name (sampler_NAME) and optionally the binding point.
- **Inputs**: They are not stored in the metadata, we assume that the order the user defines the inputs is the order they are in the shader.

## Stages
Stage1 -> Stage2 -- NebulaShaderCompiler
Stage2 -> Stage2.1 -- NebulaShaderCompiler
Stage2.1 -> Stage2.2 -- NebulaShaderConductor
(Stage1 --> Stage2.2) -- NebulaShaderBuilder
(Stage2.2 --> Stage3) -- NebulaShaderBuilder
Stage3 --> NebulaShaderLoader
