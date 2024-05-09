#include "Transform.hpp"

#include <Eigen/Geometry>

namespace Nebula
{
    void Transform::Translate(float transX, float transY, float transZ) noexcept
    {
        m_Translation.x() += transX;
        m_Translation.y() += transY;
        m_Translation.z() += transZ;
    }

    void Transform::Translate(const Vector3f& translation) noexcept { m_Translation += translation; }

    void Transform::Rotate(float angle, const Vector3f& axis) noexcept
    {
        auto rotation = Eigen::AngleAxisf(angle, axis);
        m_Rotation    = m_Rotation * Quaternionf(rotation);
    }

    void Transform::Scale(float scaleX, float scaleY, float scaleZ) noexcept
    {
        m_Scale.x() *= scaleX;
        m_Scale.y() *= scaleY;
        m_Scale.z() *= scaleZ;
    }

    void Transform::Scale(const Vector3f& scale) noexcept
    {
        m_Scale.x() *= scale.x();
        m_Scale.y() *= scale.y();
        m_Scale.z() *= scale.z();
    }

    Matrix4f Transform::GetModelMatrix() const noexcept
    {
        Eigen::Transform<float, 3, Eigen::Affine> transform = Eigen::Transform<float, 3, Eigen::Affine>::Identity();

        transform.scale(m_Scale);
        transform.rotate(m_Rotation);
        transform.translate(m_Translation);

        return transform.matrix();
    }
} // namespace Nebula
