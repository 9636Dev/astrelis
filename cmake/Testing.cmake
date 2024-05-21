# ==============================================================================
# Testing Setup
# ==============================================================================

if (NEBULA_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(vendor/googletest)
    message(STATUS "NebulaEngine: Testing is enabled")
else()
    message(STATUS "NebulaEngine: Testing is disabled")
endif()
