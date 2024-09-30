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

    template<std::size_t N, typename T> class Vector {
    public:
        // Constructor for uniform value
        explicit Vector(T value = 0) : m_Vector(value) {
        }

        // Variadic constructor to forward to glm vector
        template<typename... Args>
        constexpr explicit Vector(Args... args) : m_Vector(std::forward<Args>(args)...) {
            static_assert(sizeof...(args) == N, "Invalid number of arguments");
        }

        Vector(T valX, T valY)
            requires(N == 2)
            : m_Vector(valX, valY) {
        }

        Vector(T valX, T valY, T valZ)
            requires(N == 3)
            : m_Vector(valX, valY, valZ) {
        }

        Vector(T valX, T valY, T valZ, T valW)
            requires(N == 4)
            : m_Vector(valX, valY, valZ, valW) {
        }

        // Copy constructor
        explicit Vector(const glm::vec<N, T, glm::defaultp>& vec) : m_Vector(vec) {
        }

        // Assignment operators for chaining
        Vector& operator+=(const Vector& other) {
            m_Vector += other.m_Vector;
            return *this;
        }

        Vector& operator-=(const Vector& other) {
            m_Vector -= other.m_Vector;
            return *this;
        }

        Vector& operator*=(T scalar) {
            m_Vector *= scalar;
            return *this;
        }

        Vector& operator/=(T scalar) {
            m_Vector /= scalar;
            return *this;
        }

        // Access underlying glm vector for direct manipulation
        glm::vec<N, T, glm::defaultp>& GetGLMVector() {
            return m_Vector;
        }
        const glm::vec<N, T, glm::defaultp>& GetGLMVector() const {
            return m_Vector;
        }

        T& operator[](std::size_t index) {
            return m_Vector[index];
        }

        // Overloaded arithmetic operators for clean syntax and chaining
        friend Vector operator+(Vector lhs, const Vector& rhs) {
            lhs += rhs;
            return lhs;
        }

        friend Vector operator-(Vector lhs, const Vector& rhs) {
            lhs -= rhs;
            return lhs;
        }

        friend Vector operator*(Vector lhs, T scalar) {
            lhs *= scalar;
            return lhs;
        }

        friend Vector operator/(Vector lhs, T scalar) {
            lhs /= scalar;
            return lhs;
        }

    private:
        glm::vec<N, T, glm::defaultp> m_Vector;
    };

    // Common vector type aliases
    using Vec2f = Vector<2, float>;
    using Vec3f = Vector<3, float>;
    using Vec4f = Vector<4, float>;
    using Vec2i = Vector<2, std::int32_t>;
    using Vec3i = Vector<3, std::int32_t>;
    using Vec4i = Vector<4, std::int32_t>;
    using Vec2u = Vector<2, std::uint32_t>;
    using Vec3u = Vector<3, std::uint32_t>;
    using Vec4u = Vector<4, std::uint32_t>;

    template<std::size_t C, std::size_t R, typename T> class Matrix {
    public:
        // Default constructor
        explicit Matrix(T value = 1.0F) : m_Matrix(value) {
        }

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        Matrix(const glm::mat<C, R, T, glm::defaultp>& mat) : m_Matrix(mat) {
        }

        // Variadic constructor forwarding to glm::mat constructor
        template<typename... Args>
        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        constexpr Matrix(Args... args) : m_Matrix(std::forward<Args>(args)...) {
        }

        Matrix(std::initializer_list<std::initializer_list<T>> list) : m_Matrix(list) {
            // We can't directly forward the list to glm::mat constructor
        }

        // Assignment operators for chaining
        Matrix& operator+=(const Matrix& other) {
            m_Matrix += other.m_Matrix;
            return *this;
        }

        Matrix& operator-=(const Matrix& other) {
            m_Matrix -= other.m_Matrix;
            return *this;
        }

        Matrix& operator*=(const Matrix& other) {
            m_Matrix *= other.m_Matrix;
            return *this;
        }

        Matrix& operator*=(T scalar) {
            m_Matrix *= scalar;
            return *this;
        }

        // Access underlying glm matrix for direct manipulation
        glm::mat<C, R, T, glm::defaultp>& GetGLMMatrix() {
            return m_Matrix;
        }
        const glm::mat<C, R, T, glm::defaultp>& GetGLMMatrix() const {
            return m_Matrix;
        }

        // Overloaded arithmetic operators for chaining
        friend Matrix operator+(Matrix lhs, const Matrix& rhs) {
            lhs += rhs;
            return lhs;
        }

        friend Matrix operator-(Matrix lhs, const Matrix& rhs) {
            lhs -= rhs;
            return lhs;
        }

        friend Matrix operator*(Matrix lhs, const Matrix& rhs) {
            lhs *= rhs;
            return lhs;
        }

        friend Matrix operator*(Matrix lhs, T scalar) {
            lhs *= scalar;
            return lhs;
        }

        void Translated(const Vector<3, T>& translation)
            requires(C == 4 && R == 4)
        {
            m_Matrix = glm::translate(m_Matrix, translation.GetGLMVector());
        }

        Matrix Translate(const Vector<3, T>& translation) const
            requires(C == 4 && R == 4)
        {
            return Matrix(glm::translate(m_Matrix, translation.GetGLMVector()));
        }

        void Rotated(T angle, const Vector<3, T>& axis)
            requires(C == 4 && R == 4)
        {
            m_Matrix = glm::rotate(m_Matrix, angle, axis.GetGLMVector());
        }

        Matrix Rotate(T angle, const Vector<3, T>& axis) const
            requires(C == 4 && R == 4)
        {
            return Matrix(glm::rotate(m_Matrix, angle, axis.GetGLMVector()));
        }

        void Scaled(const Vector<3, T>& scale)
            requires(C == 4 && R == 4)
        {
            m_Matrix = glm::scale(m_Matrix, scale.GetGLMVector());
        }

        Matrix Scale(const Vector<3, T>& scale) const
            requires(C == 4 && R == 4)
        {
            return Matrix(glm::scale(m_Matrix, scale.GetGLMVector()));
        }

        void Normalized() {
            m_Matrix = glm::normalize(m_Matrix);
        }

        Matrix Normalize() const {
            return Matrix(glm::normalize(m_Matrix));
        }

        Vector<C, T> operator[](std::size_t index) {
            return Vector<C, T>(m_Matrix[index]);
        }

    private:
        glm::mat<C, R, T, glm::defaultp> m_Matrix;
    };

    // Common matrix type aliases
    using Mat2f = Matrix<2, 2, float>;
    using Mat3f = Matrix<3, 3, float>;
    using Mat4f = Matrix<4, 4, float>;
    using Mat2i = Matrix<2, 2, std::int32_t>;
    using Mat3i = Matrix<3, 3, std::int32_t>;
    using Mat4i = Matrix<4, 4, std::int32_t>;
    using Mat2u = Matrix<2, 2, std::uint32_t>;
    using Mat3u = Matrix<3, 3, std::uint32_t>;
    using Mat4u = Matrix<4, 4, std::uint32_t>;
    using Mat4u = Matrix<4, 4, std::uint32_t>;
} // namespace Astrelis
