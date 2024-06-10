#pragma once

#include "Eigen/Core"
#include <Eigen/Dense>

namespace Nebula
{
    template<typename T, int N>
    using Vector = Eigen::Matrix<T, N, 1>;

    template<typename T, int R, int C>
    using Matrix = Eigen::Matrix<T, R, C>;

    template<typename T>
    using Quaternion = Eigen::Quaternion<T>;

    using Vector2f = Vector<float, 2>;
    using Vector3f = Vector<float, 3>;
    using Vector4f = Vector<float, 4>;

    using Matrix2f = Matrix<float, 2, 2>;
    using Matrix3f = Matrix<float, 3, 3>;
    using Matrix4f = Matrix<float, 4, 4>;

    using Quaternionf = Quaternion<float>;
} // namespace Nebula
