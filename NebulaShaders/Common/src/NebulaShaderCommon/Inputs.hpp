#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Nebula::Shader
{
    struct StringInput
    {
        std::string Name;
        std::string Type;
        std::string Semantic;

        StringInput(std::string name, std::string type, std::string semantic) :
            Name(std::move(name)),
            Type(std::move(type)),
            Semantic(std::move(semantic))
        {
        }
    };

    enum class InputType
    {
        Float,
        Float2,
        Float3,
        Float4,
    };

    inline std::optional<InputType> StringToInputType(const std::string& type)
    {
        static const std::unordered_map<std::string, InputType> map = {
            {"float",  InputType::Float },
            {"float2", InputType::Float2},
            {"float3", InputType::Float3},
            {"float4", InputType::Float4},
        };

        auto iter = map.find(type);
        if (iter != map.end())
        {
            return iter->second;
        }

        return std::nullopt;
    }

    enum class InputSemantic
    {
        Position,
        Normal,
        TexCoord,
    };

    inline std::optional<InputSemantic> StringToInputSemantic(const std::string& semantic)
    {
        static const std::unordered_map<std::string, InputSemantic> map = {
            {"POSITION", InputSemantic::Position},
            {"NORMAL",   InputSemantic::Normal  },
            {"TEXCOORD", InputSemantic::TexCoord},
        };

        auto iter = map.find(semantic);
        if (iter != map.end())
        {
            return iter->second;
        }

        return std::nullopt;
    }

    struct Input
    {
        InputType Type;
        std::string Name;
        InputSemantic Semantic;

        Input(InputType type, std::string name, InputSemantic semantic) :
            Type(type),
            Name(std::move(name)),
            Semantic(semantic)
        {
        }

        template<typename Archive> void serialize(Archive& archive) { archive(Type, Name, Semantic); }
    };

} // namespace Nebula::Shader
