# ==============================================================================
# Default configuration for NebulaEngine
# ==============================================================================


# =======================
# Logging Configuration
# =======================

# Log Levels:
# 0 - Trace
# 1 - Debug
# 2 - Info
# 3 - Warn
# 4 - Error
# 5 - Fatal
# 6 - Off

set(NEBULA_DEBUG_LOGLEVEL "0" CACHE STRING "The log level for debug messages")
set_property(CACHE NEBULA_DEBUG_LOGLEVEL PROPERTY STRINGS "0" "1" "2" "3" "4" "5" "6")
set(NEBULA_RELEASE_LOGLEVEL "2" CACHE STRING "The log level for release messages")
set_property(CACHE NEBULA_RELEASE_LOGLEVEL PROPERTY STRINGS "0" "1" "2" "3" "4" "5" "6")
set(NEBULA_DIST_LOGLEVEL "3" CACHE STRING "The log level for dist messages")
set_property(CACHE NEBULA_DIST_LOGLEVEL PROPERTY STRINGS "0" "1" "2" "3" "4" "5" "6")


# =======================
# Build Type Configuration
# =======================

set(NEBULA_BUILD_TYPE "Debug" CACHE STRING "The build type for NebulaEngine")
set_property(CACHE NEBULA_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "Dist")

if(NEBULA_BUILD_TYPE STREQUAL "Debug")
    set(NEBULA_LOGLEVEL ${NEBULA_DEBUG_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type for NebulaEngine" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but NEBULA_BUILD_TYPE is set to Debug. This may cause unexpected behavior.")
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type for NebulaEngine" FORCE)
    endif()
elseif(NEBULA_BUILD_TYPE STREQUAL "Release")
    set(NEBULA_LOGLEVEL ${NEBULA_RELEASE_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for NebulaEngine" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but NEBULA_BUILD_TYPE is set to Release. This may cause unexpected behavior.")
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for NebulaEngine" FORCE)
    endif()
elseif(NEBULA_BUILD_TYPE STREQUAL "Dist")
    set(NEBULA_LOGLEVEL ${NEBULA_DIST_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Dist" CACHE STRING "The build type for NebulaEngine" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Dist")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but NEBULA_BUILD_TYPE is set to Dist. This may cause unexpected behavior.")
        set(CMAKE_BUILD_TYPE "Dist" CACHE STRING "The build type for NebulaEngine" FORCE)
    endif()
endif()


# =======================
# Testing Configuration
# =======================

option(NEBULA_TESTING "Enable testing for NebulaEngine" OFF) # This disables tests globally, because this will stop googletest from being built

# =======================
# Project Configuration
# =======================

set(NEBUlA_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "The root directory for NebulaEngine")
