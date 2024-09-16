#pragma once

#include <string>
#include <vector>

namespace AstrelisEditor {
    class ShaderCompiler {
    public:
        ShaderCompiler();
        ~ShaderCompiler();
        ShaderCompiler(const ShaderCompiler&) = delete;
        ShaderCompiler& operator=(const ShaderCompiler&) = delete;
        ShaderCompiler(ShaderCompiler&&) = delete;
        ShaderCompiler& operator=(ShaderCompiler&&) = delete;

        std::vector<std::uint32_t> CompileHLSLToSPIRV(const std::string& hlslSource, const std::string& entryPoint, const std::string& targetProfile);
    private:
        class Impl;
        Impl* m_Impl;
    };
} // namespace AstrelisEditor
