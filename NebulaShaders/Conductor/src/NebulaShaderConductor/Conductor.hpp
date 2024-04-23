#pragma once

#include "Core.hpp"

#include <memory>

namespace Nebula::ShaderConductor
{
    class NEBULA_SHADER_CONDUCTOR_API ShaderConductor
    {
    public:
        ShaderConductor();

    private:
        class Impl;
        std::unique_ptr<Impl> m_Impl;
    };
} // namespace Nebula::ShaderConductor
