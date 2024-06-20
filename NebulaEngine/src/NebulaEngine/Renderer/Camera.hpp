#pragma once

#include <glm/glm.hpp>

namespace Nebula
{
    class Camera
    {
    public:
        Camera() = default;
        explicit Camera(const glm::mat4& projectionMatrix) : m_ProjectionMatrix(projectionMatrix) {}

        void SetProjectionMatrix(const glm::mat4& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }
        void SetViewMatrix(const glm::mat4& viewMatrix) { m_ViewMatrix = viewMatrix; }

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

    private:
        glm::mat4 m_ViewMatrix = glm::mat4(1.0F);
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0F);
    };
} // namespace Nebula
