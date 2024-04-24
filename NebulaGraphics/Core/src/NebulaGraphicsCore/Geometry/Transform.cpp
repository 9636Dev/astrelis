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

    void Transform::Translate(const Vector3f& translation) noexcept
    {
        m_Translation += translation;
    }

    void Transform::Rotate(float angle, const Vector3f& axis) noexcept
    {
        auto rotation = Eigen::AngleAxisf(angle, axis);
        m_Rotation = m_Rotation * Quaternionf(rotation);
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
        Matrix4f scaleMatrix = Matrix4f::Identity();
        scaleMatrix(0, 0) = m_Scale.x();
        scaleMatrix(1, 1) = m_Scale.y();
        scaleMatrix(2, 2) = m_Scale.z();

        Matrix4f rotationMatrix = Matrix4f::Identity();
        rotationMatrix.block<3, 3>(0, 0) = m_Rotation.toRotationMatrix();

        Matrix4f translationMatrix = Matrix4f::Identity();
        translationMatrix.block<3, 1>(0, 3) = m_Translation;

        return translationMatrix * rotationMatrix * scaleMatrix;
    }
} // namespace Nebula
