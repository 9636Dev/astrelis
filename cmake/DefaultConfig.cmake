# This is the global config, you can override these in the subdirectories

# This is for all the projects if their type is set to "Inherit"
set(NEBULA_LIBRARY_TYPE "Shared" CACHE STRING "Type of library to build")
set_property(CACHE NEBULA_LIBRARY_TYPE PROPERTY STRINGS "Shared" "Static" "Inherit")

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(NEBULA_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
else()
    set(NEBULA_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()
set_property(CACHE NEBULA_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

set(NEBULA_SHADER_DIRECTORY "${NEBULA_RUNTIME_DIR}/resources/shaders" CACHE STRING "Directory for shaders")

option(NEBULA_USE_PTHREADS "Use pthreads for threading" ON)

# This can be tuned for the specific project
option(NEBULA_BUILD_TESTS "Build tests" ON)

option(NEBULA_ENABLE_CLANG_FORMAT "Enable clang-format" ON)
option(NEBULA_ENABLE_PEDANTIC "Enable pedantic warnings" ON)
