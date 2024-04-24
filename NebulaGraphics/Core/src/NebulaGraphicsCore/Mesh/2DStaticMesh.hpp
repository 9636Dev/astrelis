#pragma once

#include "../Core.hpp"
#include "BaseMesh.hpp"

namespace Nebula
{
    class NEBULA_GRAPHICS_CORE_API Static2DMesh : public BaseMesh
    {
    public:
        Static2DMesh()                               = default;
        ~Static2DMesh() override                     = default;
        Static2DMesh(const Static2DMesh& other)      = default;
        Static2DMesh& operator=(const Static2DMesh&) = default;
        Static2DMesh(Static2DMesh&& other)           = delete;
        Static2DMesh& operator=(Static2DMesh&&)      = delete;

        [[nodiscard]] bool IsStatic() const noexcept override { return true; }

        [[nodiscard]] bool IsInterleaved() const noexcept override { return true; }

        [[nodiscard]] bool Is3D() const noexcept override { return false; }

        std::vector<float>& Vertices() noexcept { return m_VertexData; };
        std::vector<uint32_t>& Indices() noexcept { return m_IndexData; };
    };
} // namespace Nebula
