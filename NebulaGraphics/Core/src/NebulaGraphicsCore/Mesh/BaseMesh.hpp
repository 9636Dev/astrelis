#pragma once

#include <cstdint>
#include <vector>

namespace Nebula
{
    class BaseMesh
    {
    public:
        BaseMesh()                           = default;
        virtual ~BaseMesh()                  = default;
        BaseMesh(const BaseMesh& other)      = default;
        BaseMesh& operator=(const BaseMesh&) = default;
        BaseMesh(BaseMesh&& other)           = delete;
        BaseMesh& operator=(BaseMesh&&)      = delete;

        [[nodiscard]] virtual bool IsStatic() const noexcept = 0;
        [[nodiscard]] bool IsDynamic() const noexcept { return !IsStatic(); }

        [[nodiscard]] virtual bool IsInterleaved() const noexcept = 0;
        [[nodiscard]] bool IsPacked() const noexcept { return !IsInterleaved(); }

        [[nodiscard]] virtual bool Is3D() const noexcept = 0;
        [[nodiscard]] bool Is2D() const noexcept { return !Is3D(); }

        [[nodiscard]] virtual const std::vector<float>& GetVertexData() const noexcept { return m_VertexData; }
        [[nodiscard]] virtual const std::vector<std::uint32_t>& GetIndexData() const noexcept { return m_IndexData; }
    protected:
        std::vector<float> m_VertexData; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
        std::vector<std::uint32_t> m_IndexData; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
    };
} // namespace Nebula
