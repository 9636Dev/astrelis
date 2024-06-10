#pragma once

#include "NebulaEngine/Core/Math.hpp"

namespace Nebula
{
    class Transform
    {
    public:
        Transform();
        void Translate(const Vector3f& translation);
        void Rotate(const Quaternionf& rotation);
        void Rotate(const Vector3f& axis, float angle);
        void Scale(const Vector3f& scale);
        Matrix4f ToMatrix() const;

        const Vector3f& GetPosition() const { return m_Position; }
        const Quaternionf& GetRotation() const { return m_Rotation; }
        const Vector3f& GetScale() const { return m_Scale; }

    private:
        Vector3f m_Position;
        Quaternionf m_Rotation;
        Vector3f m_Scale;
    };
} // namespace Nebula

