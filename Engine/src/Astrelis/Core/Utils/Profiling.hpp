#pragma once

#include <Tracy/Tracy.hpp>

#define ASTRELIS_PROFILE_FUNCTION()  ZoneScoped
#define ASTRELIS_PROFILE_SCOPE(name) ZoneScopedN(name)
#define ASTRELIS_PROFILE_END_FRAME() FrameMark
