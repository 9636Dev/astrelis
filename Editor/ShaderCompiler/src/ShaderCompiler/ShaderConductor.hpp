#pragma once

#include "Astrelis/Core/Result.hpp"
#include "Astrelis/Renderer/ShaderFormat.hpp"

#include <string>
#include <vector>

namespace AstrelisEditor {
    class ShaderConductor {
    public:
        struct HLSLSource {
            std::wstring          FileName = L"shader.hlsl";
            std::string           Source;
            std::string           Entrypoint;
            Astrelis::ShaderStage Stage;
            std::uint32_t         VersionMajor = 6;
            std::uint32_t         VersionMinor = 0;
        };

        struct HLSLOptions {
            bool          SPIRV             = true;
            std::uint32_t OptimizationLevel = 3;
        };

        ShaderConductor();
        ~ShaderConductor();
        ShaderConductor(const ShaderConductor&)            = delete;
        ShaderConductor& operator=(const ShaderConductor&) = delete;
        ShaderConductor(ShaderConductor&&)                 = delete;
        ShaderConductor& operator=(ShaderConductor&&)      = delete;

        Astrelis::Result<Astrelis::EmptyType, std::string> Init();
        void                                               Shutdown();

        Astrelis::Result<std::vector<std::uint32_t>, std::string> HLSLToSPIRV(
            const HLSLSource& source, const HLSLOptions& options);
    private:
        class Impl;
        Impl* m_Impl;
    };
} // namespace AstrelisEditor
