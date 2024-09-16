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
    class ShaderCompiler::Impl {
    public:
        Impl() {
            if (FAILED(m_DxcSupport.Initialize())) {
                throw std::runtime_error("Failed to initialize DXC support");
            }

            if (FAILED(m_DxcSupport.CreateInstance(CLSID_DxcCompiler, &m_Compiler))) {
                throw std::runtime_error("Failed to create DXC compiler");
            }

            if (FAILED(m_DxcSupport.CreateInstance(CLSID_DxcLibrary, &m_Library))) {
                throw std::runtime_error("Failed to create DXC library");
            }
        }

        ~Impl() {
            m_Compiler.Release();
            m_Library.Release();
            m_DxcSupport.Cleanup();
        }
        Impl(const Impl&)            = delete;
        Impl& operator=(const Impl&) = delete;
        Impl(Impl&&)                 = delete;
        Impl& operator=(Impl&&)      = delete;


        std::vector<std::uint32_t> CompileHLSLToSPIRV(const std::string& hlslSource,
            const std::string& entryPoint, const std::string& targetProfile) {
            CComPtr<IDxcBlobEncoding> sourceBlob;
            if (FAILED(m_Library->CreateBlobWithEncodingFromPinned(hlslSource.data(),
                    static_cast<UINT32>(hlslSource.size()), CP_UTF8, &sourceBlob))) {
                throw std::runtime_error("Failed to create blob from HLSL source");
            }

            std::vector<LPCWSTR> compileArgs = {
                L"-spirv", // This tells DXC to compile to SPIR-V
                L"-fvk-use-dx-layout", // Ensures compatibility with HLSL layout semantics in Vulkan
                L"-fspv-reflect" // Optional: if you want to enable reflection for SPIR-V
            };

            CComPtr<IDxcOperationResult> result;
            if (FAILED(m_Compiler->Compile(sourceBlob, L"shader.hlsl",
                    Unicode::UTF8ToWideStringOrThrow(entryPoint.c_str()).c_str(),
                    Unicode::UTF8ToWideStringOrThrow(targetProfile.c_str()).c_str(),
                    compileArgs.data(), static_cast<UINT32>(compileArgs.size()), nullptr, 0,
                    nullptr, &result))) {
                throw std::runtime_error("Failed to compile HLSL source");
            }

            HRESULT status = S_OK;
            if (FAILED(result->GetStatus(&status))) {
                throw std::runtime_error("Failed to get compilation status");
            }

            if (FAILED(status)) {
                CComPtr<IDxcBlobEncoding> errorBlob;
                if (FAILED(result->GetErrorBuffer(&errorBlob))) {
                    throw std::runtime_error("Failed to get error buffer");
                }

                std::string error(static_cast<const char*>(errorBlob->GetBufferPointer()),
                    errorBlob->GetBufferSize());
                throw std::runtime_error("Compilation failed: " + error);
            }

            CComPtr<IDxcBlob> shaderBlob;
            if (FAILED(result->GetResult(&shaderBlob))) {
                throw std::runtime_error("Failed to get shader blob");
            }

            return std::vector<std::uint32_t>(
                static_cast<const std::uint32_t*>(shaderBlob->GetBufferPointer()),
                static_cast<const std::uint32_t*>(shaderBlob->GetBufferPointer())
                    + shaderBlob->GetBufferSize() / sizeof(std::uint32_t));
        }

    private:
        dxc::DxcDllSupport    m_DxcSupport;
        CComPtr<IDxcCompiler> m_Compiler;
        CComPtr<IDxcLibrary>  m_Library;
    };

    ShaderCompiler::ShaderCompiler() : m_Impl(new Impl) {
    }

    ShaderCompiler::~ShaderCompiler() {
        delete m_Impl;
    }

    std::vector<std::uint32_t> ShaderCompiler::CompileHLSLToSPIRV(const std::string& hlslSource,
        const std::string& entryPoint, const std::string& targetProfile) {
        return m_Impl->CompileHLSLToSPIRV(hlslSource, entryPoint, targetProfile);
    }
} // namespace AstrelisEditor
