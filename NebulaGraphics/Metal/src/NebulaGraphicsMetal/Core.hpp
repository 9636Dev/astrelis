#pragma once

#include "NebulaCore/Core.hpp"

#ifdef NEBULA_EXPORT_DLL
#define NEBULA_GRAPHICS_METAL_API NEBULA_API_EXPORT
#else
#define NEBULA_GRAPHICS_METAL_API NEBULA_API_IMPORT
#endif
