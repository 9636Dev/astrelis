#pragma once

#ifndef NEBULA_DEBUG
    #define QPROFILE_NO_MACRO_IMPL
#endif

#include <QProfile/Macros.hpp>

#define NEBULA_PROFILE_START_SESSION(name, filepath) QPROFILE_START_SESSION(name, filepath)
#define NEBULA_PROFILE_END_SESSION()                 QPROFILE_END_SESSION()
#define NEBULA_PROFILE_SCOPE(name)                   QPROFILE_SCOPE(name)

namespace Nebula
{
    class Profiling
    {
    public:
        static bool Init();
    };
} // namespace Nebula
