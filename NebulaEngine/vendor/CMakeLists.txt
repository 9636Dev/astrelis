add_subdirectory(gsl)
add_subdirectory(spdlog)
add_subdirectory(glfw)

# ImGui
set(IMGUI_SRC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
)
#if(backend STREQUAL "GLFWOpenGL3")
#    list(APPEND IMGUI_SRC imgui/backends/imgui_impl_glfw.cpp)
#    list(APPEND IMGUI_LIB glfw)
#    list(APPEND IMGUI_DEPS glfw)
#
#    list(APPEND IMGUI_SRC imgui/backends/imgui_impl_opengl3.cpp)
#    find_package(OpenGL REQUIRED)
#    list(APPEND IMGUI_LIB OpenGL::GL)
#endif()

add_library(ImGui STATIC ${IMGUI_SRC})
target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
