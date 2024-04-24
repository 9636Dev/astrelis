#pragma once

#include "Core.hpp"

namespace Nebula::ShaderConductor
{
    class NEBULA_SHADER_CONDUCTOR_API ShaderConductor
    {
    public:
        ShaderConductor();

    private:
        class Impl;
        Impl* m_Impl;
    };
} // namespace Nebula::ShaderConductor
