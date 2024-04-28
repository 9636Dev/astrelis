#include "FileFormat.hpp"

#include <fstream>
#include <sstream>

#include "NebulaCore/Assert.hpp"
#include "NebulaShaderCompiler/ShaderCompiler.hpp"
#include "NebulaShaderConductor/Conductor.hpp"
#include "NebulaShaderConductor/TargetProfile.hpp"

#include <cereal/archives/binary.hpp>

namespace Nebula::Shader
{
    std::pair<IntermediateFormat, std::string> IntermediateFormat::FromSource(std::string source)
    {
        IntermediateFormat intermediateFormat;

        Compiler compiler(std::move(source));
        auto compilerError = compiler.Compile();
        if (compilerError.has_value())
        {
            std::stringstream sstream;
            sstream << "Failed to compile shader: " << compilerError->Message;
            auto [row, column] = compiler.GetRowColumn(compilerError->Index);
            sstream << " at " << row << ":" << column;
            return std::make_pair(std::move(intermediateFormat), sstream.str());
        }

        // Bindings and textures
        {
            ProgramMeta programMeta;
            programMeta.Name     = compiler.GetMeta().Name;
            const auto& bindings = compiler.GetBindings();
            programMeta.Bindings.reserve(bindings.size());
            for (const auto& binding : bindings)
            {
                auto bindingRes = Binding::FromStringBinding(binding);
                if (bindingRes.second != BindingError::None)
                {
                    return std::make_pair(std::move(intermediateFormat),
                                          "Failed to convert StringBinding to Binding: " +
                                              BindingErrorToString(bindingRes.second));
                }

                programMeta.Bindings.push_back(std::move(bindingRes.first));
            }

            const auto& textures = compiler.GetTextures();
            programMeta.Textures.reserve(textures.size());
            for (const auto& texture : textures)
            {
                auto textureRes = TextureBinding::FromStringBinding(texture);
                if (textureRes.second != BindingError::None)
                {
                    return std::make_pair(std::move(intermediateFormat),
                                          "Failed to convert StringBinding to TextureBinding: " +
                                              BindingErrorToString(textureRes.second));
                }

                programMeta.Textures.push_back(std::move(textureRes.first));
            }

            intermediateFormat.m_ProgramMeta = std::move(programMeta);
        }

        auto sources = compiler.GetSources();

        NEB_ASSERT(sources.Vertex.has_value(), "Vertex source is missing");
        intermediateFormat.m_VertexSource = sources.Shared.value_or("") + sources.Vertex.value();
        NEB_ASSERT(sources.Pixel.has_value(), "Pixel source is missing");
        intermediateFormat.m_PixelSource = sources.Shared.value_or("") + sources.Pixel.value();

        intermediateFormat.m_VertexEntrypoint = compiler.GetVertexEntrypoint();
        intermediateFormat.m_PixelEntrypoint  = compiler.GetPixelEntrypoint();

        return std::make_pair(intermediateFormat, "");
    }

    std::string IntermediateFormat::GenerateGlsl(const ShaderConductor::GLSLOutput& output)
    {
        if (m_GlslSource.has_value())
        {
            return "GLSL code already exists";
        }

        GLSLShaderSource glslSource;
        ShaderConductor::ShaderConductor conductor;

        ShaderConductor::ShaderInput vertexInput;
        vertexInput.Source     = m_VertexSource;
        vertexInput.Profile    = ShaderConductor::TargetProfile(ShaderStage::Vertex, 6, 0);
        vertexInput.EntryPoint = m_VertexEntrypoint;

        auto spirvVertex = conductor.CompileToSPIRV(vertexInput, output);
        if (!spirvVertex.success)
        {
            return spirvVertex.errorMessage;
        }

        ShaderConductor::ShaderInput pixelInput;
        pixelInput.Source     = m_PixelSource;
        pixelInput.Profile    = ShaderConductor::TargetProfile(ShaderStage::Pixel, 6, 0);
        pixelInput.EntryPoint = m_PixelEntrypoint;

        auto spirvPixel = conductor.CompileToSPIRV(pixelInput, output);
        if (!spirvPixel.success)
        {
            return spirvPixel.errorMessage;
        }

        auto glslVertex = ShaderConductor::ShaderConductor::CompileToGLSL(spirvVertex.spirvCode, output);
        if (!glslVertex.first.second.empty())
        {
            return glslVertex.first.first;
        }

        auto glslPixel = ShaderConductor::ShaderConductor::CompileToGLSL(spirvPixel.spirvCode, output);
        if (!glslPixel.first.second.empty())
        {
            return glslPixel.first.first;
        }

        glslSource.Version      = output.Version;
        glslSource.VertexSource = glslVertex.first.first;
        glslSource.PixelSource  = glslPixel.first.first;
        glslSource.Meta         = glslVertex.second;
        NEB_ASSERT(glslSource.Meta == glslPixel.second, "GLSL meta data does not match");

        m_GlslSource = std::move(glslSource);

        return "";
    }
    FileWriteResult IntermediateFormat::WriteToFile(const File& file) const {
        std::ofstream out(file.GetPath(), std::ios::binary);
        if (!out) {
            return {false, 0}; // Check if the file stream opened successfully
        }

        FileHeader header;
        header.Meta = m_ProgramMeta;
        header.GlslPresent = m_GlslSource.has_value();

        cereal::BinaryOutputArchive archive(out); // Bind the archive directly to the file output stream
        archive(header);

        if (m_GlslSource.has_value()) {
            archive(m_GlslSource.value());
        }

        if (!out.good()) {
            return {false, 0}; // Check for any errors
        }

        return {true, static_cast<size_t>(out.tellp())}; // Successful write and the size of data written
    }
} // namespace Nebula::Shader
