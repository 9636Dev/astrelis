#include "Conductor.hpp"

#include <dxc/dxcapi.h>
#include <dxc/Support/dxcapi.use.h>
#include <iostream>
#include <vector>
#include <string>

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

            CComPtr<IDxcCompiler> compiler;
            CComPtr<IDxcLibrary> library;

            hres = dxcSupport.CreateInstance(CLSID_DxcCompiler, &compiler);
            if (FAILED(hres))
            {
                std::cerr << "Failed to create DXC compiler" << std::endl;
                return;
            }

            hres = dxcSupport.CreateInstance(CLSID_DxcLibrary, &library);
            if (FAILED(hres))
            {
                std::cerr << "Failed to create DXC compiler" << std::endl;
                return;
            }
        }

        ~Impl()
        {
        }

    private:
    };

    ShaderConductor::ShaderConductor() : m_Impl()
    {
    }
} // namespace Nebula::ShaderConductor
