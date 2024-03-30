#pragma once

#include <NebulaCore/Core.hpp>

#ifdef NEBULA_EXPORT_DLL
#define NEBULA_GRAPHICS_OPENGL_API __declspec(dllexport)
#else
#define NEBULA_GRAPHICS_OPENGL_API __declspec(dllimport)
#endif
