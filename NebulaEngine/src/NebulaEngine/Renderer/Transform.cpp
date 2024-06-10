#include "Transform.hpp"

namespace Nebula
{
    Transform::Transform()
        : m_Position(Vector3f::Zero())
        , m_Rotation(Quaternionf::Identity())
        , m_Scale(Vector3f::Ones())
    {
    }

    void Transform::Translate(const Vector3f& translation)
    {
        m_Position += translation;
    }

    void Transform::Rotate(const Quaternionf& rotation)
    {
        m_Rotation = rotation * m_Rotation;
    }

    void Transform::Rotate(const Vector3f& axis, float angle)
    {
        Rotate(Quaternionf(Eigen::AngleAxisf(angle, axis)));
    }

    void Transform::Scale(const Vector3f& scale)
    {
        m_Scale.x() *= scale.x();
        m_Scale.y() *= scale.y();
        m_Scale.z() *= scale.z();
    }

    Matrix4f Transform::ToMatrix() const
    {
        return (Eigen::Translation3f(m_Position) * Eigen::Quaternionf(m_Rotation) * Eigen::Scaling(m_Scale)).matrix();
    }
}  // namespace Nebula
