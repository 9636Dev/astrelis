#include "Conductor.hpp"

namespace Nebula::ShaderConductor
{

    class ShaderConductor::Impl
    {
    public:
        Impl()
        {
        }

        ~Impl()
        {
        }

    private:
    };

    ShaderConductor::ShaderConductor() : m_Impl(std::make_unique<Impl>())
    {
    }
} // namespace Nebula::ShaderConductor
