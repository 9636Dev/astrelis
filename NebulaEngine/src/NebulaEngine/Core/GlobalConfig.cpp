#include "GlobalConfig.hpp"

namespace Nebula
{
#ifdef NEBULA_DEBUG
    bool GlobalConfig::s_DebugMode = true;
#else
    bool GlobalConfig::s_DebugMode = false;
#endif
} // namespace Nebula
