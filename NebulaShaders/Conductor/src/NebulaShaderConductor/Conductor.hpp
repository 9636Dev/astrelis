#pragma once

#include "Core.hpp"
#include "ShaderInput.hpp"
#include "ShaderOutput.hpp"

#include <cstdint>
#include <string>
#include <vector>

#include "NebulaShaderCommon/Meta.hpp"

namespace Nebula::ShaderConductor
{
    struct SPIRVCompilationResult
    {
        bool success;
        std::string errorMessage;
        std::vector<std::uint32_t> spirvCode;

        SPIRVCompilationResult(bool succ, std::string errMsg, std::vector<uint32_t> code) :
            success(succ),
            errorMessage(std::move(errMsg)),
            spirvCode(std::move(code))
        {
        }
    };

    class NEBULA_SHADER_CONDUCTOR_API ShaderConductor
    {
    public:
        using ShaderCompileOutput                = std::pair<std::string, std::string>;
        template<typename T> using CompileOutput = std::pair<ShaderCompileOutput, T>;
        ShaderConductor();
        ~ShaderConductor();

        ShaderConductor(const ShaderConductor&)            = default;
        ShaderConductor(ShaderConductor&&)                 = default;
        ShaderConductor& operator=(const ShaderConductor&) = default;
        ShaderConductor& operator=(ShaderConductor&&)      = default;

        SPIRVCompilationResult CompileToSPIRV(const ShaderInput& input, const ShaderOutput& output);
        static CompileOutput<Shader::GLSLMeta> CompileToGLSL(const std::vector<uint32_t>& spirv,
                                                             const GLSLOutput& output);
        static CompileOutput<Shader::HLSLMeta> CompileToHLSL(const std::vector<uint32_t>& spirv,
                                                             const HLSLOutput& output);
        static CompileOutput<Shader::MetalMeta> CompileToMsl(const std::vector<uint32_t>& spirv,
                                                             const MslOutput& output);

        bool Initialize();
    private:
        class Impl;
        Impl* m_Impl;
    };
} // namespace Nebula::ShaderConductor
