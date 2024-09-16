/**
 * @file Math.hpp
 * @brief Prevents a simple wrapper for math functions
*/

#pragma once

#define GLM_FORCE_RADIANS
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace Astrelis {
    using Vec2f = glm::vec<2, float, glm::defaultp>;
    using Vec3f = glm::vec<3, float, glm::defaultp>;
    using Vec4f = glm::vec<4, float, glm::defaultp>;
    using Vec2i = glm::vec<2, std::int32_t, glm::defaultp>;
    using Vec3i = glm::vec<3, std::int32_t, glm::defaultp>;
    using Vec4i = glm::vec<4, std::int32_t, glm::defaultp>;
    using Vec2u = glm::vec<2, std::uint32_t, glm::defaultp>;
    using Vec3u = glm::vec<3, std::uint32_t, glm::defaultp>;
    using Vec4u = glm::vec<4, std::uint32_t, glm::defaultp>;
    using Mat4f = glm::mat<4, 4, float, glm::defaultp>;

    namespace Math {
        inline Mat4f Translate(const Mat4f& matrix, const Vec3f& translation) {
            return glm::translate(matrix, translation);
        }

        inline Mat4f Rotate(const Mat4f& matrix, float angle, const Vec3f& axis) {
            return glm::rotate(matrix, angle, axis);
        }

        inline Mat4f Scale(const Mat4f& matrix, const Vec3f& scale) {
            return glm::scale(matrix, scale);
        }

        inline Mat4f Orthographic(
            float left, float right, float bottom, float top, float near, float far) {
            return glm::orthoRH_ZO(left, right, bottom, top, near, far);
        }

        inline Mat4f Perspective(float fov, float aspect, float near, float far) {
            return glm::perspective(fov, aspect, near, far);
        }

        inline Mat4f LookAt(const Vec3f& eye, const Vec3f& center, const Vec3f& upDir) {
            return glm::lookAt(eye, center, upDir);
        }

        template<typename T> inline T Mean(const std::vector<T>& values) {
            T sum = static_cast<T>(0);
            for (const auto& value : values) {
                sum += value;
            }
            return sum / static_cast<T>(values.size());
        }
    } // namespace Math
} // namespace Astrelis
