#include "UI.hpp"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

namespace Nebula
{
    void UI::Init(const Ref<Window>& window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        // Enable Docking
        ImGuiIO& imguiIo = ImGui::GetIO();
        imguiIo.IniFilename = nullptr;
        imguiIo.ConfigDockingWithShift = true;
        imguiIo.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    #ifndef NEBULA_PLATFORM_LINUX
        imguiIo.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    #else
        NEBULA_CORE_LOG_INFO("Viewports not enabled on Linux (Not supported)");
    #endif

        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window->GetNativeWindow()), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void UI::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void UI::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void UI::EndFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    #ifndef NEBULA_PLATFORM_LINUX
        auto *const windowContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(windowContext);
    #endif
    }
} // namespace Nebula
