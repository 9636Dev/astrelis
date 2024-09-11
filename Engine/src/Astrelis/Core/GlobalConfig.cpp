#include "GlobalConfig.hpp"
#include "Astrelis/Core/Base.hpp"

namespace Astrelis
{
#ifdef ASTRELIS_DEBUG
    bool GlobalConfig::s_DebugMode = true;
#else
    bool GlobalConfig::s_DebugMode = false;
#endif
} // namespace Astrelis
