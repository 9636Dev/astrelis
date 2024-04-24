#pragma once

#include "NebulaGraphicsCore/Core.hpp"
#include "NebulaGraphicsCore/Math.hpp"

namespace Nebula
{
    class NEBULA_GRAPHICS_CORE_API Transform
    {
    public:
        Transform()                              = default;
        ~Transform()                             = default;
        Transform(const Transform& other)      = default;
        Transform& operator=(const Transform&) = default;
        Transform(Transform&& other)           = delete;
        Transform& operator=(Transform&&)      = delete;

        void Translate(float transX, float transY, float transZ) noexcept;
        void Translate(const Vector3f& translation) noexcept;
        void Rotate(float angle, const Vector3f& axis) noexcept;
        void Scale(float scaleX, float scaleY, float scaleZ) noexcept;
        void Scale(const Vector3f& scale) noexcept;

        [[nodiscard]] Vector3f GetTranslation() const noexcept { return m_Translation; }
        [[nodiscard]] Quaternionf GetRotation() const noexcept { return m_Rotation; }
        [[nodiscard]] Vector3f GetScale() const noexcept { return m_Scale; }

        [[nodiscard]] Matrix4f GetModelMatrix() const noexcept;
    private:
        Vector3f m_Translation = Vector3f::Zero();
        Quaternionf m_Rotation = Quaternionf::Identity();
        Vector3f m_Scale       = Vector3f::Ones();
    };
} // namespace Nebula

