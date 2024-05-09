#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace Nebula::Shader
{
    /**
    * This stores a binding in string form , which is used when it is parsed. This needs to be type checked and converted to a more usable form.
    * @see Binding
    */
    struct StringBinding
    {
        std::string Type;
        std::string Name;
        std::string Target; // The value that will be passed to the shader, for example: "Position", "Color", "Texture"

        StringBinding(std::string type, std::string name, std::string target) :
            Type(std::move(type)),
            Name(std::move(name)),
            Target(std::move(target))
        {
        }
    };

    enum class BindingError
    {
        None,
        InvalidType,
        InvalidTarget,
    };

    inline std::string BindingErrorToString(BindingError error)
    {
        switch (error)
        {
        case BindingError::None:
            return "None";
        case BindingError::InvalidType:
            return "InvalidType";
        case BindingError::InvalidTarget:
            return "InvalidTarget";
        }
    }


    enum class BindingType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Float4x4,
        Sampler2D,
    };

    inline std::optional<BindingType> StringToBindingType(const std::string& type)
    {
        static const std::unordered_map<std::string, BindingType> map = {
            {"float",    BindingType::Float   },
            {"float2",   BindingType::Float2  },
            {"float3",   BindingType::Float3  },
            {"float4",   BindingType::Float4  },
            {"float4x4", BindingType::Float4x4},
        };

        auto iter = map.find(type);
        if (iter != map.end())
        {
            return iter->second;
        }

        return std::nullopt;
    }

    enum class BindingTarget
    {
        ModelMatrix,
        ViewMatrix,
        ProjectionMatrix,
    };

    inline std::optional<BindingTarget> StringToBindingTarget(const std::string& target)
    {
        static const std::unordered_map<std::string, BindingTarget> map = {
            {"MODEL_MATRIX",      BindingTarget::ModelMatrix     },
            {"VIEW_MATRIX",       BindingTarget::ViewMatrix      },
            {"PROJECTION_MATRIX", BindingTarget::ProjectionMatrix},
        };

        auto iter = map.find(target);
        if (iter != map.end())
        {
            return iter->second;
        }

        return std::nullopt;
    }

    struct Binding
    {
        BindingType Type;
        std::string Name;
        BindingTarget Target;

        Binding(BindingType type, std::string name, BindingTarget target) :
            Type(type),
            Name(std::move(name)),
            Target(target)
        {
        }

        Binding() : Binding(BindingType::Float, "", BindingTarget::ModelMatrix) {}

        static std::pair<Binding, BindingError> FromStringBinding(const StringBinding& stringBinding)
        {
            auto type = StringToBindingType(stringBinding.Type);
            if (!type)
            {
                return {Binding(BindingType::Float, "", BindingTarget::ModelMatrix), BindingError::InvalidType};
            }

            auto target = StringToBindingTarget(stringBinding.Target);
            if (!target)
            {
                return {Binding(BindingType::Float, "", BindingTarget::ModelMatrix), BindingError::InvalidTarget};
            }

            return {Binding(*type, stringBinding.Name, *target), BindingError::None};
        }

        template<typename Archive> void serialize(Archive& archive) { archive(Type, Name, Target); }
    };

    enum class TextureType
    {
        Sampler2D,
    };

    inline std::optional<TextureType> StringToTextureType(const std::string& type)
    {
        static const std::unordered_map<std::string, TextureType> map = {
            {"sampler2D", TextureType::Sampler2D},
        };

        auto iter = map.find(type);
        if (iter != map.end())
        {
            return iter->second;
        }

        return std::nullopt;
    }

    enum class TextureTarget
    {
        Diffuse,
    };

    inline std::optional<TextureTarget> StringToTextureTarget(const std::string& target)
    {
        static const std::unordered_map<std::string, TextureTarget> map = {
            {"DIFFUSE", TextureTarget::Diffuse},
        };

        auto iter = map.find(target);
        if (iter != map.end())
        {
            return iter->second;
        }

        return std::nullopt;
    }

    struct TextureBinding
    {
        TextureType Type;
        std::string Name;
        TextureTarget Target;

        TextureBinding(TextureType type, std::string name, TextureTarget target) :
            Type(type),
            Name(std::move(name)),
            Target(target)
        {
        }

        TextureBinding() : TextureBinding(TextureType::Sampler2D, "", TextureTarget::Diffuse) {}

        static std::pair<TextureBinding, BindingError> FromStringBinding(const StringBinding& stringBinding)
        {
            auto type = StringToTextureType(stringBinding.Type);
            if (!type)
            {
                return {TextureBinding(TextureType::Sampler2D, "", TextureTarget::Diffuse), BindingError::InvalidType};
            }

            auto target = StringToTextureTarget(stringBinding.Target);
            if (!target)
            {
                return {TextureBinding(TextureType::Sampler2D, "", TextureTarget::Diffuse),
                        BindingError::InvalidTarget};
            }

            return {TextureBinding(*type, stringBinding.Name, *target), BindingError::None};
        }

        template<typename Archive> void serialize(Archive& archive) { archive(Type, Name, Target); }
    };
} // namespace Nebula::Shader
