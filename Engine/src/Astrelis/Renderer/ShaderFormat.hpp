#pragma once

#include <cstdint>
#include <string>
#include <vector>

// The definition for the .astshader file format
// This should be in the Engine not the Editor

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>

namespace Astrelis {
    constexpr std::uint64_t GenerateFileSig(std::array<char, 8> sig) {
        std::uint64_t result = 0;
        for (std::size_t i = 0; i < sig.size(); ++i) {
            result |= static_cast<std::uint64_t>(sig[i]) << (i * 8);
        }
        return result;
    }

    enum class ShaderStage : std::uint8_t {
        Vertex,
        Fragment,
    };

    struct ShaderHeader {
        constexpr static auto Signature = GenerateFileSig({'A', 'S', 'T', 'S', 'H', 'A', 'D', 'E'});
        enum class HeaderFlags : std::uint8_t {
            None     = 0,
            SPIRV    = 1 << 0,
            HLSL     = 1 << 1,
            GLSL     = 1 << 2,
            MSL      = 1 << 3,
            Vertex   = 1 << 4,
            Fragment = 1 << 5,
        };

        std::uint64_t FileSignature = Signature;
        std::uint64_t FileVersion;
        std::string   Name;
        HeaderFlags   Flags;
    };

    inline ShaderHeader::HeaderFlags operator&(
        ShaderHeader::HeaderFlags lhs, ShaderHeader::HeaderFlags rhs) {
        using T = std::underlying_type_t<ShaderHeader::HeaderFlags>;
        return static_cast<ShaderHeader::HeaderFlags>(static_cast<T>(lhs) & static_cast<T>(rhs));
    }

    inline ShaderHeader::HeaderFlags operator|(
        ShaderHeader::HeaderFlags lhs, ShaderHeader::HeaderFlags rhs) {
        using T = std::underlying_type_t<ShaderHeader::HeaderFlags>;
        return static_cast<ShaderHeader::HeaderFlags>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    struct ShaderSource {
        struct SPIRV {
            struct Shader {
                std::vector<std::uint32_t> Data;
                std::string                Entrypoint;

                template<typename Archive> void serialize(Archive& archive) {
                    archive(Data, Entrypoint);
                }
            };
            std::vector<std::pair<ShaderStage, Shader>> Shaders;

            template<typename Archive> void serialize(Archive& archive) {
                archive(Shaders);
            }
        };

        SPIRV SourceSPIRV;
    };

    struct ShaderFormat {
        ShaderHeader Header;
        ShaderSource Source;

        template<typename Archive> void serialize(Archive& archive) {
            archive(Header.FileSignature, Header.FileVersion, Header.Name, Header.Flags);
            if ((Header.Flags & ShaderHeader::HeaderFlags::SPIRV)
                != ShaderHeader::HeaderFlags::None) {
                archive(Source.SourceSPIRV);
            }
        }
    };
} // namespace Astrelis
