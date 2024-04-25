cmake_minimum_required(VERSION 3.27)

project(ImGui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_SOURCE_DIR CACHE "Imgui Source Dir" ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

function(ImGuiAddLibrary target backend)
    set(IMGUI_TARGET_NAME ImGui_${backend})

    set(IMGUI_SRC
        ${IMGUI_SOURCE_DIR}/imgui.cpp
        ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
        ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
        ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    )
    set(IMGUI_LIB "")
    set(IMGUI_DEPS "")

    if(backend STREQUAL "GLFWOpenGL3")
        list(APPEND IMGUI_SRC ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)
        list(APPEND IMGUI_LIB glfw)
        list(APPEND IMGUI_DEPS glfw)

        list(APPEND IMGUI_SRC ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
        find_package(OpenGL REQUIRED)
        list(APPEND IMGUI_LIB OpenGL::GL)
    else()
        message(FATAL_ERROR "Unknown backend: ${backend}")
    endif()

    message(STATUS "IMGUI: ${IMGUI_SRC}")

    add_library(${IMGUI_TARGET_NAME} STATIC ${IMGUI_SRC})
    target_include_directories(${IMGUI_TARGET_NAME} PUBLIC ${IMGUI_SOURCE_DIR})

    target_link_libraries(${IMGUI_TARGET_NAME} PUBLIC ${IMGUI_LIB})
    add_dependencies(${IMGUI_TARGET_NAME} ${IMGUI_DEPS})

    target_link_libraries(${target} PRIVATE ${IMGUI_TARGET_NAME})
    add_dependencies(${target} ${IMGUI_TARGET_NAME})
endfunction()
