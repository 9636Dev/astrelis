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
    using ColMatrix = Eigen::Matrix<Type, Rows, Cols>;

    using CMatrix2f = ColMatrix<float, 2, 2>;
    using CMatrix3f = ColMatrix<float, 3, 3>;
    using CMatrix4f = ColMatrix<float, 4, 4>;

    template<typename Type, int Rows, int Cols>
    using RowMatrix = Eigen::Matrix<Type, Rows, Cols, Eigen::RowMajor>;

    using RMatrix2f = RowMatrix<float, 2, 2>;
    using RMatrix3f = RowMatrix<float, 3, 3>;
    using RMatrix4f = RowMatrix<float, 4, 4>;

    // Normal matrices are row major, since we are using HLSL, and by convention, HLSL uses row major matrices
    using Matrix2f = RMatrix2f;
    using Matrix3f = RMatrix3f;
    using Matrix4f = RMatrix4f;

    template<typename Type>
    using Quaternion = Eigen::Quaternion<Type>;

    using Quaternionf = Quaternion<float>;
} // namespace Nebula
