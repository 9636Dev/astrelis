#pragma once

#include "Astrelis/Core/Math.hpp"

namespace Astrelis {
    class Camera {
    public:
        Camera() = default;

        explicit Camera(const Mat4f& projectionMatrix) : m_ProjectionMatrix(projectionMatrix) {
        }

        void SetProjectionMatrix(const Mat4f& projectionMatrix) {
            m_ProjectionMatrix = projectionMatrix;
        }

        void SetViewMatrix(const Mat4f& viewMatrix) {
            m_ViewMatrix = viewMatrix;
        }

        const Mat4f& GetProjectionMatrix() const {
            return m_ProjectionMatrix;
        }

        const Mat4f& GetViewMatrix() const {
            return m_ViewMatrix;
        }
    private:
        Mat4f m_ViewMatrix       = Mat4f(1.0F);
        Mat4f m_ProjectionMatrix = Mat4f(1.0F);
    };
} // namespace Astrelis
