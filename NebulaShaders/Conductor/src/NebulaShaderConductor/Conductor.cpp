#include "Conductor.hpp"

#include "NebulaShaderConductor/ShaderInput.hpp"
#include "NebulaShaderConductor/ShaderOutput.hpp"

#include <dxc/Support/dxcapi.use.h>
#include <dxc/dxcapi.h>
#include <iostream>
#include <string>
#include <vector>

namespace Nebula::ShaderConductor
{

    class ShaderConductor::Impl
    {
    public:
        Impl()
        {
            dxc::DxcDllSupport dxcSupport;
            HRESULT hres = dxcSupport.Initialize();
            if (FAILED(hres))
            {
                std::cerr << "Failed to initialize DXC" << std::endl;
                return;
            }

            hres = dxcSupport.CreateInstance(CLSID_DxcCompiler, &m_Compiler);
            if (FAILED(hres))
            {
                std::cerr << "Failed to create DXC compiler" << std::endl;
                return;
            }

            hres = dxcSupport.CreateInstance(CLSID_DxcLibrary, &m_Library);
            if (FAILED(hres))
            {
                std::cerr << "Failed to create DXC compiler" << std::endl;
                return;
            }
        }

        CComPtr<IDxcBlobEncoding> StringToBlob(const std::string &str)
        {
            CComPtr<IDxcBlobEncoding> blob;
            HRESULT hres = m_Library->CreateBlobWithEncodingFromPinned(str.c_str(), str.size(), CP_UTF8, &blob);
            if (FAILED(hres))
            {
                std::cerr << "Failed to create blob from string" << std::endl;
                return nullptr;
            }
            return blob;
        }

        static std::vector<LPCWSTR> GenerateArgsFromIO([[maybe_unused]] const ShaderInput& input, const ShaderOutput& output)
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

        CComPtr<IDxcOperationResult> InternalCompile(const CComPtr<IDxcBlobEncoding>& source, const ShaderInput& input, std::vector<LPCWSTR> args)
        {
            CComPtr<IDxcOperationResult> result;
            m_Compiler->Compile(
                source,  // pSource
                input.FileName.c_str(),  // pSourceName
                input.EntryPoint.c_str(),  // pEntryPoint
                input.Profile.ToString().c_str(),  // pTargetProfile
                args.data(), // pArguments
                static_cast<UINT32>(args.size()), // argCount
                nullptr, // pDefines
                0, // defineCount
                nullptr, // pIncludeHandler
                &result // ppResult
            );
            return result;
        }

        ~Impl()
        {
        }

        Impl(const Impl &) = delete;
        Impl &operator=(const Impl &) = delete;
        Impl(Impl &&) = delete;
        Impl &operator=(Impl &&) = delete;

        CComPtr<IDxcCompiler> m_Compiler;
        CComPtr<IDxcLibrary> m_Library;
    };

    ShaderConductor::ShaderConductor() : m_Impl()
    {
    }
} // namespace Nebula::ShaderConductor
