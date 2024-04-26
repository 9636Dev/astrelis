#pragma once

#include "NebulaCore/Core.hpp"
#ifdef NEBUlA_EXPORT_DLL
#define NEBULA_SHADER_BUILDER_API NEBULA_API_EXPORT
#else
#define NEBULA_SHADER_BUILDER_API NEBULA_API_IMPORT
#endif
