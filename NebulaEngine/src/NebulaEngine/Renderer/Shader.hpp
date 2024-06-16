#pragma once

#include "NebulaEngine/Core/Math.hpp"
#include "NebulaEngine/Core/Result.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

#include <string_view>
namespace Nebula
{
    class Shader
    {
    public:
        Shader() = default;
        virtual ~Shader() = default;
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader& operator=(Shader&&) = delete;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetMat4(std::string_view name, Matrix4f value) = 0;

        static Ptr<Shader> Create(std::string name, std::string_view vertexSrc, std::string_view fragmentSrc);
    };
} // namespace Nebula
