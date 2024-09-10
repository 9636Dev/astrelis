project(NebulaEngine_Sandbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SANDOX_SOURCES
    src/Sandbox/SandboxApp.cpp
    src/Sandbox/SandboxApplication.hpp
    src/Sandbox/SandboxApplication.cpp
    src/Sandbox/SandboxLayer.hpp
    src/Sandbox/SandboxLayer.cpp
)
add_executable(Astrelis_Sandbox ${NEBULA_SANDOX_SOURCES})

target_link_libraries(Astrelis_Sandbox Astrelis::Engine)
