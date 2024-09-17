#include "ShaderCompiler.hpp"

#include "Astrelis/Renderer/ShaderFormat.hpp"

#include <set>

namespace AstrelisEditor {
    Astrelis::Result<Astrelis::EmptyType, std::string> ShaderCompiler::Init() {
        return m_ShaderConductor.Init();
    }

    void ShaderCompiler::Shutdown() {
        m_ShaderConductor.Shutdown();
    }

    Astrelis::Result<Astrelis::ShaderFormat, std::string> ShaderCompiler::CompileShader(
        const std::vector<SourceStage>& sources) {
        Astrelis::ShaderHeader header = {
            .FileSignature = Astrelis::ShaderHeader::Signature,
            .FileVersion   = 1,
            .Name          = "Shader", // TODO
        };

        {
            // Check arguments
            std::set<Astrelis::ShaderStage> stages;

            if (sources.empty()) {
                return "No sources provided";
            }

            for (const auto& src : sources) {
                if (!stages.insert(src.Stage).second) {
                    return "Multiple sources with the same stage";
                }

                switch (src.Stage) {
                case Astrelis::ShaderStage::Vertex:
                    header.Flags = header.Flags | Astrelis::ShaderHeader::HeaderFlags::Vertex;
                    break;
                case Astrelis::ShaderStage::Fragment:
                    header.Flags = header.Flags | Astrelis::ShaderHeader::HeaderFlags::Fragment;
                    break;
                default:
                    return "Unsupported stage";
                }
            }
        }

        Astrelis::ShaderSource shaderSource;

        {
            // SPIRV Generation
            header.Flags = header.Flags | Astrelis::ShaderHeader::HeaderFlags::SPIRV;


            std::vector<std::pair<Astrelis::ShaderStage, Astrelis::ShaderSource::SPIRV::Shader>>
                spirv(sources.size());

            ShaderConductor::HLSLOptions options;
            options.SPIRV             = true;
            options.OptimizationLevel = 3; // TODO: Make this configurable

            for (const SourceStage& source : sources) {
                ShaderConductor::HLSLSource src;
                src.Source       = source.Source;
                src.Entrypoint   = source.Entrypoint;
                src.FileName     = L"shader.hlsl";
                src.Stage        = source.Stage;
                src.VersionMajor = 6;
                src.VersionMinor = 0;

                auto result = m_ShaderConductor.HLSLToSPIRV(src, options);
                if (result.IsErr()) {
                    return result.UnwrapErr();
                }

                spirv.emplace_back(source.Stage,
                    Astrelis::ShaderSource::SPIRV::Shader {
                        .Data       = std::move(result.Unwrap()),
                        .Entrypoint = source.Entrypoint,
                    });
            }

            shaderSource.SourceSPIRV = Astrelis::ShaderSource::SPIRV {
                .Shaders = std::move(spirv),
            };
        }

        return Astrelis::ShaderFormat {
            .Header = header,
            .Source = shaderSource,
        };
    }
} // namespace AstrelisEditor
