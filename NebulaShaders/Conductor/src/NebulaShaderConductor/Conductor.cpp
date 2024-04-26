#include "Conductor.hpp"

#include "NebulaCore/Log.hpp"
#include "spirv_cross.hpp"
#include "spirv_cross_error_handling.hpp"

#include <dxc/Support/dxcapi.use.h>
#include <dxc/dxcapi.h>

#include <spirv_glsl.hpp>
#include <spirv_hlsl.hpp>
#include <spirv_msl.hpp>

#include <string>
#include <vector>

namespace Nebula::ShaderConductor
{

    class ShaderConductor::Impl
    {
    public:
        Impl()
        {
            HRESULT hres = m_DxcSupport.Initialize();
            if (FAILED(hres))
            {
                NEB_CORE_LOG_ERROR("Failed to initialize DXC support");
            }

            hres = m_DxcSupport.CreateInstance(CLSID_DxcCompiler, &m_Compiler);
            if (FAILED(hres))
            {
                NEB_CORE_LOG_ERROR("Failed to create DXC compiler");
                return;
            }

            hres = m_DxcSupport.CreateInstance(CLSID_DxcLibrary, &m_Library);
            if (FAILED(hres))
            {
                NEB_CORE_LOG_ERROR("Failed to create DXC library");
                return;
            }
        }

        bool Initialize() { return m_Compiler != nullptr && m_Library != nullptr; }

        CComPtr<IDxcBlobEncoding> StringToBlob(const std::string& str)
        {
            CComPtr<IDxcBlobEncoding> blob;
            HRESULT hres = m_Library->CreateBlobWithEncodingFromPinned(str.c_str(), str.size(), CP_UTF8, &blob);
            if (FAILED(hres))
            {
                NEB_CORE_LOG_DEBUG("Failed to create blob from string");
                return nullptr;
            }
            return blob;
        }

        static std::vector<LPCWSTR> GenerateArgsFromIO([[maybe_unused]] const ShaderInput& input,
                                                       const ShaderOutput& output)
        {
            std::vector<LPCWSTR> vector = {
                L"-spirv",
            };

            switch (output.Optimization)
            {
            case OptimizationLevel::None:
                vector.push_back(L"-Od");
                break;
            case OptimizationLevel::Level1:
                vector.push_back(L"-O1");
                break;
            case OptimizationLevel::Level2:
                vector.push_back(L"-O2");
                break;
            case OptimizationLevel::Level3:
                vector.push_back(L"-O3");
                break;
            }

            return vector;
        }

        CComPtr<IDxcOperationResult> InternalCompile(const CComPtr<IDxcBlobEncoding>& source,
                                                     const ShaderInput& input,
                                                     std::vector<LPCWSTR> args)
        {
            std::wstring filename   = std::wstring(input.FileName.begin(), input.FileName.end());
            std::wstring entryPoint = std::wstring(input.EntryPoint.begin(), input.EntryPoint.end());

            CComPtr<IDxcOperationResult> result;
            m_Compiler->Compile(source,                           // pSource
                                filename.c_str(),                 // pSourceName
                                entryPoint.c_str(),               // pEntryPoint
                                input.Profile.ToString().c_str(), // pTargetProfile
                                args.data(),                      // pArguments
                                static_cast<UINT32>(args.size()), // argCount
                                nullptr,                          // pDefines
                                0,                                // defineCount
                                nullptr,                          // pIncludeHandler
                                &result                           // ppResult
            );

            return result;
        }

        SPIRVCompilationResult CompileToSPIRV(const ShaderInput& input, const ShaderOutput& output)
        {
            CComPtr<IDxcBlobEncoding> source = StringToBlob(input.Source);
            if (source == nullptr)
            {
                return {false, "Failed to create blob from source", {}};
            }

            std::vector<LPCWSTR> args           = GenerateArgsFromIO(input, output);
            CComPtr<IDxcOperationResult> result = InternalCompile(source, input, args);

            HRESULT status = 0;
            result->GetStatus(&status);
            if (FAILED(status))
            {
                CComPtr<IDxcBlobEncoding> errorsBlob;
                result->GetErrorBuffer(&errorsBlob);
                std::string errorMsg(static_cast<char*>(errorsBlob->GetBufferPointer()), errorsBlob->GetBufferSize());
                return {false, errorMsg, {}};
            }

            CComPtr<IDxcBlob> outputBlob;
            result->GetResult(&outputBlob);
            if (outputBlob->GetBufferSize() % sizeof(uint32_t) != 0)
            {
                return {false, "SPIR-V output blob size is not aligned to 32-bit words, which is unexpected.", {}};
            }

            const auto* data = static_cast<const uint32_t*>(outputBlob->GetBufferPointer());
            size_t size      = outputBlob->GetBufferSize() / sizeof(uint32_t);

            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            std::vector<uint32_t> spirv(data, data + size);
            return {true, "", std::move(spirv)};
        }

        ~Impl()
        {
            // Release the compiler and library
            m_Compiler.Release();
            m_Library.Release();
        }

        Impl(const Impl&)            = delete;
        Impl& operator=(const Impl&) = delete;
        Impl(Impl&&)                 = delete;
        Impl& operator=(Impl&&)      = delete;

        CComPtr<IDxcCompiler> m_Compiler;
        CComPtr<IDxcLibrary> m_Library;
        dxc::DxcDllSupport m_DxcSupport;
    };

    ShaderConductor::ShaderConductor() : m_Impl(new Impl()) {}

    ShaderConductor::~ShaderConductor() { delete m_Impl; }

    SPIRVCompilationResult ShaderConductor::CompileToSPIRV(const ShaderInput& input, const ShaderOutput& output)
    {
        return m_Impl->CompileToSPIRV(input, output);
    }

    bool ShaderConductor::Initialize() { return m_Impl->Initialize(); }

    struct GLSLResource
    {
        // ID
        std::uint32_t ID;
        std::string Name;
        std::uint32_t Set;
        std::uint32_t Binding;
    };

    // TODO(9636D): Include metadata as well on the output
    ShaderConductor::CompileOutput<GLSLMeta> ShaderConductor::CompileToGLSL(const std::vector<std::uint32_t>& spirv,
                                                                  const GLSLOutput& output)
    {
        constexpr auto UniformBufferPrefix = "ubo_";
        constexpr auto SamplerPrefix       = "sampler_";

        try
        {
            GLSLMeta meta;
            spirv_cross::CompilerGLSL compiler(spirv);
            spirv_cross::CompilerGLSL::Options options;

            options.version                          = output.Version;
            options.es                               = output.GLES;
            options.force_zero_initialized_variables = true;
            options.enable_420pack_extension         = output.Enable420PackExtension;

            bool CanUseUniformBufferBinding = options.version >= 420 || options.enable_420pack_extension;
            bool CanUseSamplerBinding       = options.version >= 420 || options.enable_420pack_extension;

            std::vector<GLSLResource> glResources;
            {
                const auto& resources = compiler.get_shader_resources();

                for (const auto& buffer : resources.uniform_buffers)
                {
                    auto binding = compiler.get_decoration(buffer.id, spv::DecorationBinding);
                    // The naming convention is type.{OriginalName}
                    // We need to extract the original name
                    auto name = buffer.name;
                    if (name.starts_with("type."))
                    {
                        name = name.substr(5);
                    }

                    auto newname = UniformBufferPrefix + name;
                    compiler.set_name(buffer.id, newname);

                    std::optional<std::uint32_t> bindingOpt = CanUseUniformBufferBinding ? std::make_optional(binding) : std::nullopt;
                    GLSLMeta::UniformBuffer ubo(newname, bindingOpt);
                    meta.UniformBuffers.insert({name, std::move(ubo)});
                }

                for (const auto& texture : resources.separate_images)
                {
                    auto set     = compiler.get_decoration(texture.id, spv::DecorationDescriptorSet);
                    auto binding = compiler.get_decoration(texture.id, spv::DecorationBinding);
                    glResources.push_back({texture.id, texture.name, set, binding});
                }
            }

            compiler.set_common_options(options);
            compiler.build_combined_image_samplers();

            {
                const auto& resources = compiler.get_shader_resources();
                for (std::size_t i = 0; i < resources.sampled_images.size(); ++i)
                {
                    const auto& texture    = resources.sampled_images[i];
                    const auto& glResource = glResources[i];

                    std::optional<std::uint32_t> binding = CanUseSamplerBinding ? std::make_optional(glResource.Binding) : std::nullopt;
                    auto newname = SamplerPrefix + glResource.Name;
                    auto sampler = GLSLMeta::Sampler(newname, binding);
                    meta.Samplers.insert({glResource.Name, std::move(sampler)});
                    compiler.set_name(texture.id, newname);
                    compiler.set_decoration(texture.id, spv::DecorationDescriptorSet, glResource.Set);
                    compiler.set_decoration(texture.id, spv::DecorationBinding, glResource.Binding);
                }
            }

            return {{compiler.compile().c_str(), ""}, meta};
        }
        catch (const spirv_cross::CompilerError& e)
        {
            return {{"", e.what()}, {}};
        }
    }

    ShaderConductor::CompileOutput<HLSLMeta> ShaderConductor::CompileToHLSL(const std::vector<std::uint32_t>& spirv,
                                                                  const HLSLOutput& output)
    {
        try
        {
            spirv_cross::CompilerHLSL compiler(spirv);
            spirv_cross::CompilerHLSL::Options options;

            options.shader_model = output.HLSLVersion;

            return {{compiler.compile().c_str(), ""}, {}};
        }
        catch (const spirv_cross::CompilerError& e)
        {
            return {{"", e.what()}, {}};
        }
    }

    ShaderConductor::CompileOutput<MetalMeta> ShaderConductor::CompileToMetal(const std::vector<std::uint32_t>& spirv,
                                                                   const MetalOutput& output)
    {
        try
        {
            spirv_cross::CompilerMSL compiler(spirv);
            spirv_cross::CompilerMSL::Options options;

            options.msl_version = output.MslVersion;
            // Currently, SPIRV-Cross does not support options for Metal

            return {{compiler.compile().c_str(), ""}, {}};
        }
        catch (const spirv_cross::CompilerError& e)
        {
            return {{"", e.what()}, {}};
        }
    }
} // namespace Nebula::ShaderConductor
