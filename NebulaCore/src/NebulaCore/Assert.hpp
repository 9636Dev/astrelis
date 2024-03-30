#pragma once

#include "Util.hpp"

#ifdef NEBULA_DEBUG
#ifdef NEB_INCLUDE_LIBC_ASSERT
// We don't use NEBULA_LOG here
#include <iostream>

#define NEB_ASSERT(x, ...) { if(!(x)) { std::cerr << "Assertion Failed: " << __VA_ARGS__ << std::endl; NEB_DEBUGBREAK(); } }
#else
#include "Log.hpp"

#define NEB_ASSERT(x, ...) { if(!(x)) { NEB_CORE_LOG_FATAL(__VA_ARGS__); NEB_DEBUGBREAK(); } }

#endif
#else
#define NEB_ASSERT(x, ...)
#endif
