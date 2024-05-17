# ==============================================================================
# Testing Setup
# ==============================================================================

if (NEBULA_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(vendor/googletest)
else()
    message(STATUS "NebulaEngine: Testing is disabled")
endif()
