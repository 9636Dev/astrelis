add_subdirectory(gsl)
add_subdirectory(spdlog)
add_subdirectory(glm)
add_subdirectory(glfw)
add_subdirectory(Eigen)
add_subdirectory(qprofile)
add_subdirectory(entt)

# GLAD
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

# ImGui
set(IMGUI_SRC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp

    imgui/backends/imgui_impl_glfw.cpp
    $<$<BOOL:${NEBULA_RENDERER_OPENGL}>:
        imgui/backends/imgui_impl_opengl3.cpp
    >
    $<$<BOOL:${NEBULA_RENDERER_VULKAN}>:
        imgui/backends/imgui_impl_vulkan.cpp
    >
)

add_library(ImGui STATIC ${IMGUI_SRC})
target_link_libraries(ImGui glad glfw)
target_include_directories(ImGui PUBLIC imgui)
