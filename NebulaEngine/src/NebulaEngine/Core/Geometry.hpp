#pragma once

#include <cstdint>

#include "Types.hpp"

namespace Nebula
{
    template<Types::Arithmetic T> struct Point2D
    {
        T X;
        T Y;

        constexpr Point2D(T paramX, T paramY) noexcept : X(paramX), Y(paramY) {}

        constexpr Point2D() noexcept : Point2D(0, 0) {}

        constexpr ~Point2D() noexcept                         = default;
        constexpr Point2D(const Point2D&) noexcept            = default;
        constexpr Point2D& operator=(const Point2D&) noexcept = default;
        constexpr Point2D(Point2D&&) noexcept                 = default;
        constexpr Point2D& operator=(Point2D&&) noexcept      = default;
    };

    template<Types::Arithmetic T> struct Point3D
    {
        T X;
        T Y;
        T Z;

        constexpr Point3D(T paramX, T paramY, T paramZ) noexcept : X(paramX), Y(paramY), Z(paramZ) {}

        constexpr Point3D() noexcept : Point3D(0, 0, 0) {}

        constexpr ~Point3D() noexcept                         = default;
        constexpr Point3D(const Point3D&) noexcept            = default;
        constexpr Point3D& operator=(const Point3D&) noexcept = default;
        constexpr Point3D(Point3D&&) noexcept                 = default;
        constexpr Point3D& operator=(Point3D&&) noexcept      = default;
    };

    template<Types::Arithmetic T> struct Dimension2D
    {
        T Width;
        T Height;

        constexpr Dimension2D(T width, T height) noexcept : Width(width), Height(height) {}

        constexpr Dimension2D() noexcept : Dimension2D(0, 0) {}

        constexpr ~Dimension2D() noexcept                             = default;
        constexpr Dimension2D(const Dimension2D&) noexcept            = default;
        constexpr Dimension2D& operator=(const Dimension2D&) noexcept = default;
        constexpr Dimension2D(Dimension2D&&) noexcept                 = default;
        constexpr Dimension2D& operator=(Dimension2D&&) noexcept      = default;
    };

    template<Types::Arithmetic T> struct Dimension3D
    {
        T Width;
        T Height;
        T Depth;

        constexpr Dimension3D(T width, T height, T depth) noexcept : Width(width), Height(height), Depth(depth) {}

        constexpr Dimension3D() noexcept : Dimension3D(0, 0, 0) {}

        constexpr ~Dimension3D() noexcept                             = default;
        constexpr Dimension3D(const Dimension3D&) noexcept            = default;
        constexpr Dimension3D& operator=(const Dimension3D&) noexcept = default;
        constexpr Dimension3D(Dimension3D&&) noexcept                 = default;
        constexpr Dimension3D& operator=(Dimension3D&&) noexcept      = default;
    };

    template<Types::Arithmetic T> struct Rect2D
    {
        Point2D<T> Position;
        Dimension2D<T> Size;

        constexpr Rect2D(Point2D<T> point, Dimension2D<T> dimensions) noexcept : Position(point), Size(dimensions) {}

        constexpr Rect2D(T posX, T posY, T width, T height) noexcept : Position(posX, posY), Size(width, height) {}

        constexpr Rect2D() noexcept                         = default;
        constexpr Rect2D(const Rect2D&) noexcept            = default;
        constexpr Rect2D& operator=(const Rect2D&) noexcept = default;
        constexpr Rect2D(Rect2D&&) noexcept                 = default;
        constexpr Rect2D& operator=(Rect2D&&) noexcept      = default;
        constexpr ~Rect2D() noexcept                        = default;

        [[nodiscard]] T X() const noexcept { return Position.X; }

        [[nodiscard]] T Y() const noexcept { return Position.Y; }

        [[nodiscard]] T Width() const noexcept { return Size.Width; }

        [[nodiscard]] T Height() const noexcept { return Size.Height; }
    };

    template<Types::Arithmetic T> struct Rect3D
    {
        Point3D<T> Position;
        Dimension3D<T> Size;

        constexpr Rect3D(Point3D<T> point, Dimension3D<T> dimensions) noexcept : Position(point), Size(dimensions) {}

        constexpr Rect3D(T posX, T posY, T posZ, T width, T height, T depth) noexcept :
            Position(posX, posY, posZ),
            Size(width, height, depth)
        {
        }

        constexpr Rect3D() noexcept                         = default;
        constexpr Rect3D(const Rect3D&) noexcept            = default;
        constexpr Rect3D& operator=(const Rect3D&) noexcept = default;
        constexpr Rect3D(Rect3D&&) noexcept                 = default;
        constexpr Rect3D& operator=(Rect3D&&) noexcept      = default;
        constexpr ~Rect3D() noexcept                        = default;

        [[nodiscard]] T X() const noexcept { return Position.X; }

        [[nodiscard]] T Y() const noexcept { return Position.Y; }

        [[nodiscard]] T Z() const noexcept { return Position.Z; }

        [[nodiscard]] T Width() const noexcept { return Size.Width; }

        [[nodiscard]] T Height() const noexcept { return Size.Height; }

        [[nodiscard]] T Depth() const noexcept { return Size.Depth; }
    };

    using Point2Di = Point2D<int32_t>;
    using Point2Du = Point2D<uint32_t>;
    using Point2Df = Point2D<float>;
    using Point2Dd = Point2D<double>;

    using Point3Di = Point3D<int32_t>;
    using Point3Du = Point3D<uint32_t>;
    using Point3Df = Point3D<float>;
    using Point3Dd = Point3D<double>;

    using Dimension2Di = Dimension2D<std::int32_t>;
    using Dimension2Du = Dimension2D<std::uint32_t>;
    using Dimension2Df = Dimension2D<float>;
    using Dimension2Dd = Dimension2D<double>;

    using Dimension3Di = Dimension3D<std::int32_t>;
    using Dimension3Du = Dimension3D<std::uint32_t>;
    using Dimension3Df = Dimension3D<float>;
    using Dimension3Dd = Dimension3D<double>;

    using Rect2Di = Rect2D<std::int32_t>;
    using Rect2Du = Rect2D<std::uint32_t>;
    using Rect2Df = Rect2D<float>;
    using Rect2Dd = Rect2D<double>;

    using Rect3Di = Rect3D<std::int32_t>;
    using Rect3Du = Rect3D<std::uint32_t>;
    using Rect3Df = Rect3D<float>;
    using Rect3Dd = Rect3D<double>;
} // namespace Nebula
