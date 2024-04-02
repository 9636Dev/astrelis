#pragma once

#include <Eigen/Dense>

namespace Nebula
{
    template<typename Type, int Size>
    using Vector = Eigen::Vector<Type, Size>;

    using Vector2f = Vector<float, 2>;
    using Vector3f = Vector<float, 3>;
    using Vector4f = Vector<float, 4>;

    template<typename Type, int Rows, int Cols>
    using Matrix = Eigen::Matrix<Type, Rows, Cols>;

    using Matrix2f = Matrix<float, 2, 2>;
    using Matrix3f = Matrix<float, 3, 3>;
    using Matrix4f = Matrix<float, 4, 4>;

    template<typename Type, int Rows, int Cols>
    using RowMatrix = Eigen::Matrix<Type, Rows, Cols, Eigen::RowMajor>;

    using RMatrix2f = RowMatrix<float, 2, 2>;
    using RMatrix3f = RowMatrix<float, 3, 3>;
    using RMatrix4f = RowMatrix<float, 4, 4>;

    template<typename Type>
    using Quaternion = Eigen::Quaternion<Type>;

    using Quaternionf = Quaternion<float>;
} // namespace Nebula
