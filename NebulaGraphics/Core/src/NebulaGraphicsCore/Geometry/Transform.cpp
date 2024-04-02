#include "Transform.hpp"

namespace Nebula
{
    void Transform2D::Translate(float transX, float transY) noexcept
    {
        m_Translation.x() += transX;
        m_Translation.y() += transY;
    }

    void Transform2D::Translate(const Vector2f& translation) noexcept
    {
        m_Translation += translation;
    }

    void Transform2D::Rotate(float angle) noexcept
    {
        m_Rotation += angle;
    }

    void Transform2D::Scale(float scaleX, float scaleY) noexcept
    {
        m_Scale.x() *= scaleX;
        m_Scale.y() *= scaleY;
    }

    void Transform2D::Scale(const Vector2f& scale) noexcept
    {
        m_Scale.x() *= scale.x();
        m_Scale.y() *= scale.y();
    }

    [[nodiscard]] Vector2f Transform2D::GetTranslation() const noexcept
    {
        return m_Translation;
    }

    [[nodiscard]] float Transform2D::GetRotation() const noexcept
    {
        return m_Rotation;
    }

    [[nodiscard]] Vector2f Transform2D::GetScale() const noexcept
    {
        return m_Scale;
    }
} // namespace Nebula
