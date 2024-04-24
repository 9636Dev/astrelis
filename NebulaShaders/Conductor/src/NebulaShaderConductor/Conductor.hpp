#pragma once

#include "Core.hpp"
#include "ShaderInput.hpp"
#include "ShaderOutput.hpp"

#include <string>
#include <vector>

namespace Nebula::ShaderConductor
{
    struct SPIRVCompilationResult
    {
        bool success;
        std::string errorMessage;
        std::vector<uint32_t> spirvCode;

        SPIRVCompilationResult(bool succ, std::string errMsg, std::vector<uint32_t> code)
            : success(succ), errorMessage(std::move(errMsg)), spirvCode(std::move(code)) {}
    };

    class NEBULA_SHADER_CONDUCTOR_API ShaderConductor
    {
    public:
        using CompileOutput = std::pair<std::string, std::string>;
        ShaderConductor();
        ~ShaderConductor();

        ShaderConductor(const ShaderConductor&) = default;
        ShaderConductor(ShaderConductor&&) = default;
        ShaderConductor& operator=(const ShaderConductor&) = default;
        ShaderConductor& operator=(ShaderConductor&&) = default ;

        SPIRVCompilationResult CompileToSPIRV(const ShaderInput& input, const ShaderOutput& output);
        static CompileOutput CompileToGLSL(const std::vector<uint32_t>& spirv, const GLSLOutput& output);

        bool Initialize();

    private:
        class Impl;
        Impl* m_Impl;
    };
} // namespace Nebula::ShaderConductor
