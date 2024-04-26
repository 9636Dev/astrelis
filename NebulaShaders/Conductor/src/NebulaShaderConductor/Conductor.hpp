#pragma once

#include "Core.hpp"
#include "ShaderInput.hpp"
#include "ShaderOutput.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <optional>

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

    struct GLSLMeta
    {
        struct UniformBuffer
        {
            std::string Name; // New uniform buffer name
            std::optional<std::uint32_t> Binding;

            UniformBuffer(std::string name, std::optional<std::uint32_t> binding) :
                Name(std::move(name)),
                Binding(binding)
            {
            }

            ~UniformBuffer() = default;
            UniformBuffer(const UniformBuffer&) = default;
            UniformBuffer(UniformBuffer&&) = default;
            UniformBuffer& operator=(const UniformBuffer&) = default;
            UniformBuffer& operator=(UniformBuffer&&) = default;
        };

        struct Sampler
        {
            std::string Name; // New sampler name
            std::optional<std::uint32_t> Binding;

            Sampler(std::string name, std::optional<std::uint32_t> binding) :
                Name(std::move(name)),
                Binding(binding)
            {
            }

            ~Sampler() = default;
            Sampler(const Sampler&) = default;
            Sampler(Sampler&&) = default;
            Sampler& operator=(const Sampler&) = default;
            Sampler& operator=(Sampler&&) = default;
        };

        // Original uniform buffer name -> uniform buffer
        std::map<std::string, UniformBuffer> UniformBuffers;
        // Original sampler name -> sampler
        std::map<std::string, Sampler> Samplers;

        GLSLMeta() = default;
    };

    struct HLSLMeta
    {

    };

    struct MetalMeta
    {

    };

    class NEBULA_SHADER_CONDUCTOR_API ShaderConductor
    {
    public:
        using ShaderCompileOutput = std::pair<std::string, std::string>;
        template <typename T>
        using CompileOutput = std::pair<ShaderCompileOutput, T>;
        ShaderConductor();
        ~ShaderConductor();

        ShaderConductor(const ShaderConductor&)            = default;
        ShaderConductor(ShaderConductor&&)                 = default;
        ShaderConductor& operator=(const ShaderConductor&) = default;
        ShaderConductor& operator=(ShaderConductor&&)      = default;

        SPIRVCompilationResult CompileToSPIRV(const ShaderInput& input, const ShaderOutput& output);
        static CompileOutput<GLSLMeta> CompileToGLSL(const std::vector<uint32_t>& spirv, const GLSLOutput& output);
        static CompileOutput<HLSLMeta> CompileToHLSL(const std::vector<uint32_t>& spirv, const HLSLOutput& output);
        static CompileOutput<MetalMeta> CompileToMetal(const std::vector<uint32_t>& spirv, const MetalOutput& output);

        bool Initialize();
    private:
        class Impl;
        Impl* m_Impl;
    };
} // namespace Nebula::ShaderConductor
