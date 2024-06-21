add_subdirectory(spdlog)
add_subdirectory(glm)
add_subdirectory(glfw)
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
target_link_libraries(ImGui
    $<$<BOOL:${NEBULA_RENDERER_OPENGL}>:glad glfw>
    $<$<BOOL:${NEBULA_RENDERER_VULKAN}>:Vulkan::Vulkan>
)
target_include_directories(ImGui PUBLIC imgui)

# STB
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb)

# ImageMagick, precompiled binaries
#find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
