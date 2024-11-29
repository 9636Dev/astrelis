#pragma once

#include "Astrelis/Core/Result.hpp"
#include "Astrelis/Renderer/ShaderFormat.hpp"

#include "ShaderConductor.hpp"

namespace AstrelisEditor {
    class ShaderCompiler {
    public:
        ShaderCompiler() = default;

        Astrelis::Result<Astrelis::EmptyType, std::string> Init();
        void                                               Shutdown();

        struct SourceStage {
            std::string           Source;
            Astrelis::ShaderStage Stage;
            std::string           Entrypoint;
        };

        Astrelis::Result<Astrelis::ShaderFormat, std::string> CompileShader(
            const std::vector<SourceStage>& sources);

    private:
        ShaderConductor m_ShaderConductor;
    };
} // namespace AstrelisEditor
