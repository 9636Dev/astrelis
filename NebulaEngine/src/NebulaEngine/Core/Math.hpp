/**
 * @file Math.hpp
 * @brief Prevents a simple wrapper for math functions
*/

#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace Nebula
{
    using vec2  = glm::vec2;
    using vec3  = glm::vec3;
    using vec4  = glm::vec4;
    using ivec2 = glm::ivec2;
    using ivec3 = glm::ivec3;
    using ivec4 = glm::ivec4;
    using mat4  = glm::mat4;

    namespace Math
    {
        inline mat4 Translate(const mat4& matrix, const vec3& translation)
        {
            return glm::translate(matrix, translation);
        }

        inline mat4 Rotate(const mat4& matrix, float angle, const vec3& axis)
        {
            return glm::rotate(matrix, angle, axis);
        }

        inline mat4 Scale(const mat4& matrix, const vec3& scale) { return glm::scale(matrix, scale); }

        inline mat4 Orthographic(float left, float right, float bottom, float top, float near, float far)
        {
            return glm::ortho(left, right, bottom, top, near, far);
        }

        inline mat4 Perspective(float fov, float aspect, float near, float far)
        {
            return glm::perspective(fov, aspect, near, far);
        }

        inline mat4 LookAt(const vec3& eye, const vec3& center, const vec3& upDir)
        {
            return glm::lookAt(eye, center, upDir);
        }
    } // namespace Math
} // namespace Nebula
