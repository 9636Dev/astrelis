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
        std::string Type;
        std::string Name;
        std::string Semantic;

        StringInput(std::string type, std::string name, std::string semantic) :
            Type(std::move(type)),
            Name(std::move(name)),
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
        OutputPosition,
        Normal,
        TexCoord,
    };

    inline std::optional<InputSemantic> StringToInputSemantic(const std::string& semantic)
    {
        static const std::unordered_map<std::string, InputSemantic> map = {
            {"POSITION", InputSemantic::Position},
            {"OUTPUT_POSITION", InputSemantic::OutputPosition},
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

    enum class InputError
    {
        None,
        InvalidType,
        InvalidSemantic,
    };

    inline std::string InputErrorToString(InputError error)
    {
        switch (error)
        {
        case InputError::None:
            return "None";
        case InputError::InvalidType:
            return "InvalidType";
        case InputError::InvalidSemantic:
            return "InvalidSemantic";
        }
    }

    struct Input
    {
        InputType Type;
        std::string Name;
        InputSemantic Semantic;

        Input() :
            Type(InputType::Float),
            Semantic(InputSemantic::Position)
        {
        }
        Input(InputType type, std::string name, InputSemantic semantic) :
            Type(type),
            Name(std::move(name)),
            Semantic(semantic)
        {
        }

        inline static std::pair<Input, InputError> FromStringInput(const StringInput& input)
        {
            auto typeRes = StringToInputType(input.Type);
            if (!typeRes.has_value())
            {
                return std::make_pair(Input(InputType::Float, "", InputSemantic::Position), InputError::InvalidType);
            }

            auto semanticRes = StringToInputSemantic(input.Semantic);
            if (!semanticRes.has_value())
            {
                return std::make_pair(Input(InputType::Float, "", InputSemantic::Position), InputError::InvalidSemantic);
            }

            return std::make_pair(Input(typeRes.value(), input.Name, semanticRes.value()), InputError::None);
        }

        template<typename Archive> void serialize(Archive& archive) { archive(Type, Name, Semantic); }
    };

} // namespace Nebula::Shader
