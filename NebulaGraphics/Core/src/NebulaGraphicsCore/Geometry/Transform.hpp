#pragma once

#include "NebulaGraphicsCore/Core.hpp"
#include "NebulaGraphicsCore/Math.hpp"

namespace Nebula
{
    class NEBULA_GRAPHICS_CORE_API Transform2D
    {
    public:
        Transform2D()                              = default;
        ~Transform2D()                             = default;
        Transform2D(const Transform2D& other)      = default;
        Transform2D& operator=(const Transform2D&) = default;
        Transform2D(Transform2D&& other)           = delete;
        Transform2D& operator=(Transform2D&&)      = delete;

        void Translate(float transX, float transY) noexcept;
        void Translate(const Vector2f& translation) noexcept;
        void Rotate(float angle) noexcept;
        void Scale(float scaleX, float scaleY) noexcept;
        void Scale(const Vector2f& scale) noexcept;

        [[nodiscard]] Vector2f GetTranslation() const noexcept;
        [[nodiscard]] float GetRotation() const noexcept;
        [[nodiscard]] Vector2f GetScale() const noexcept;
    private:
        Vector2f m_Translation = Vector2f::Zero();
        float m_Rotation       = 0.0F;
        Vector2f m_Scale       = Vector2f::Ones();
    };
} // namespace Nebula

