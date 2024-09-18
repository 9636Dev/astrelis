#include "ShaderConductor.hpp"

#include <dxc/Support/Global.h>
#include <dxc/Support/Unicode.h>
#include <dxc/Support/WinIncludes.h>
#include <dxc/Support/dxcapi.use.h>
#include <dxc/WinAdapter.h>
#include <dxc/dxcapi.h>
#include <spirv_cross.hpp>
#include <spirv_cross_error_handling.hpp>
#include <spirv_glsl.hpp>
#include <spirv_hlsl.hpp>
#include <spirv_msl.hpp>
#include <string>
#include <vector>

namespace AstrelisEditor {
    class ShaderConductor::Impl {
    public:
        Impl() = default;

        Astrelis::Result<std::vector<std::uint32_t>, std::string> HLSLToSPIRV(
            const HLSLSource& source, const HLSLOptions& options) {
            CComPtr<IDxcBlobEncoding> sourceBlob;
            if (FAILED(m_Library->CreateBlobWithEncodingFromPinned(source.Source.data(),
                    static_cast<UINT32>(source.Source.size()), CP_UTF8, &sourceBlob))) {
                return "Failed to create blob from HLSL source";
            }

            std::vector<LPCWSTR> compileArgs;
            if (options.SPIRV) {
                compileArgs.push_back(L"-spirv");
            }

            if (options.OptimizationLevel > 5) {
                return "Optimization level must be between 0 and 5";
            }
            std::wstring optimization = L"-O" + std::to_wstring(options.OptimizationLevel);
            compileArgs.push_back(optimization.c_str());

            std::vector<DxcDefine> defines;

            std::string targetProfile;
            switch (source.Stage) {
            case Astrelis::ShaderStage::Vertex:
                targetProfile = "vs_6_0";
                defines.push_back({
                    .Name  = L"VERTEX_SHADER",
                    .Value = L"1",
                });
                break;
            case Astrelis::ShaderStage::Fragment:
                targetProfile = "ps_6_0";
                defines.push_back({
                    .Name  = L"FRAGMENT_SHADER",
                    .Value = L"1",
                });
                break;
            default:
                return "Unsupported shader stage";
            }

            CComPtr<IDxcOperationResult> result;
            if (FAILED(m_Compiler->Compile(sourceBlob, source.FileName.c_str(),
                    Unicode::UTF8ToWideStringOrThrow(source.Entrypoint.c_str()).c_str(),
                    Unicode::UTF8ToWideStringOrThrow(targetProfile.c_str()).c_str(),
                    compileArgs.data(), static_cast<UINT32>(compileArgs.size()), defines.data(),
                    defines.size(), nullptr, &result))) {
                return "Failed to compile HLSL source";
            }

            HRESULT status = S_OK;
            if (FAILED(result->GetStatus(&status))) {
                return "Failed to get compilation status";
            }

            if (FAILED(status)) {
                CComPtr<IDxcBlobEncoding> errorBlob;
                if (FAILED(result->GetErrorBuffer(&errorBlob))) {
                    return "Failed to get error buffer";
                }

                std::string error(static_cast<const char*>(errorBlob->GetBufferPointer()),
                    errorBlob->GetBufferSize());
                return "Compilation failed: " + error;
            }

            CComPtr<IDxcBlob> shaderBlob;
            if (FAILED(result->GetResult(&shaderBlob))) {
                return "Failed to get shader blob";
            }

            return std::vector<std::uint32_t>(
                static_cast<const std::uint32_t*>(shaderBlob->GetBufferPointer()),
                static_cast<const std::uint32_t*>(shaderBlob->GetBufferPointer())
                    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                    + shaderBlob->GetBufferSize() / sizeof(std::uint32_t));
        }

        Astrelis::Result<Astrelis::EmptyType, std::string> Init() {
            if (FAILED(m_DxcSupport.Initialize())) {
                return "Failed to initialize DXC support";
            }

            if (FAILED(m_DxcSupport.CreateInstance(CLSID_DxcCompiler, &m_Compiler))) {
                return "Failed to create DXC compiler";
            }

            if (FAILED(m_DxcSupport.CreateInstance(CLSID_DxcLibrary, &m_Library))) {
                return "Failed to create DXC library";
            }

            return Astrelis::EmptyType();
        }

        void Shutdown() {
            m_Compiler.Release();
            m_Library.Release();
            m_DxcSupport.Cleanup();
        }

    private:
        dxc::DxcDllSupport    m_DxcSupport;
        CComPtr<IDxcCompiler> m_Compiler;
        CComPtr<IDxcLibrary>  m_Library;
    };

    ShaderConductor::ShaderConductor() : m_Impl(new Impl) {
    }

    ShaderConductor::~ShaderConductor() {
        delete m_Impl;
    }

    Astrelis::Result<Astrelis::EmptyType, std::string> ShaderConductor::Init() {
        return m_Impl->Init();
    }

    void ShaderConductor::Shutdown() {
        m_Impl->Shutdown();
    }

    Astrelis::Result<std::vector<std::uint32_t>, std::string> ShaderConductor::HLSLToSPIRV(
        const HLSLSource& source, const HLSLOptions& options) {
        return m_Impl->HLSLToSPIRV(source, options);
    }
} // namespace AstrelisEditor
