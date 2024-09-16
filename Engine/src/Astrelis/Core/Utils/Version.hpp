#pragma once

/// @file Version.hpp
/// @brief Contains the version of the Astrelis library.
/// @details This file contains the version of the Astrelis library, which is used in the library and can be accessed by the user.

/// @def ASTRELIS_VERSION_TYPE
/// @brief The type of the version of the library
/// @details This is the type of the version of the library, such as Alpha, Beta, Release Candidate, etc.

/// @def ASTRELIS_VERSION_MAJOR
/// @brief The major version of the library

/// @def ASTRELIS_VERSION_MINOR
/// @brief The minor version of the library

/// @def ASTRELIS_VERSION_PATCH
/// @brief The patch version of the library

/// @def ASTRELIS_VERSION_STRING
/// @brief The string representation of the version of the library

#define ASTRELIS_VERSION_TYPE "Alpha"
#define ASTRELIS_VERSION_MAJOR 0
#define ASTRELIS_VERSION_MINOR 0
#define ASTRELIS_VERSION_PATCH 1

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define ASTRELIS_VERSION_STRING \
    ASTRELIS_VERSION_TYPE \
    " " TOSTRING(ASTRELIS_VERSION_MAJOR) "." TOSTRING(ASTRELIS_VERSION_MINOR) "." TOSTRING( \
        ASTRELIS_VERSION_PATCH)
