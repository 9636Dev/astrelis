#pragma once

#include <optional>
#include <string>
#include <utility>

#include "Core.hpp"
#include "File.hpp"
#include "NebulaIO/File.hpp"
#include "NebulaShaderCommon/ShaderMeta.hpp"
#include "NebulaShaderConductor/ShaderOutput.hpp"

namespace Nebula::Shader
{
    class NEBULA_SHADER_BUILDER_API IntermediateFormat
    {
    public:
        static std::pair<IntermediateFormat, std::string> FromSource(std::string source);

        /**
        * @brief Generate the GLSL code from the intermediate format, will remove existing GLSL code if it exists
        * @return The error message if there was an error
        */
        [[nodiscard]] std::string GenerateGlsl(const ShaderConductor::GLSLOutput& output);


        [[nodiscard]] FileWriteResult WriteToFile(const File& file) const;
    private:
        // These variables are not stored in the intermediate format
        std::string m_VertexSource;
        std::string m_PixelSource;
        std::string m_VertexEntrypoint;
        std::string m_PixelEntrypoint;

        // Variables below this line are stored in the intermediate format
        ProgramMeta m_ProgramMeta;
        std::optional<GLSLShaderSource> m_GlslSource;
    };
} // namespace Nebula::Shader
