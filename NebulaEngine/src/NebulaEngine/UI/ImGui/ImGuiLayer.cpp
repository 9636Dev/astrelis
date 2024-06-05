#include "ImGuiLayer.hpp"

#include "NebulaEngine/Core/Base.hpp"
#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Events/Event.hpp"

#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Nebula
{
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    void ImGuiLayer::OnAttach()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& imguiIO = ImGui::GetIO();
        (void)imguiIO;
        imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if ((imguiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
        {
            style.WindowRounding              = 0.0F;
            style.Colors[ImGuiCol_WindowBg].w = 1.0F;
        }

        // Setup Platform/Renderer bindings
        auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        if (m_BlockEvents)
        {
            ImGuiIO& imguiIo = ImGui::GetIO();
            event.Handled |= event.IsInCategory(EventCategory::Mouse) && imguiIo.WantCaptureMouse;
            event.Handled |= event.IsInCategory(EventCategory::Keyboard) && imguiIo.WantCaptureKeyboard;
        }
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void ImGuiLayer::Begin()
    {
        NEBULA_CORE_ASSERT(glfwGetCurrentContext() != nullptr, "No current OpenGL context");

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void ImGuiLayer::End()
    {
        ImGuiIO& imguiIo    = ImGui::GetIO();
        Application& app    = Application::Get();
        imguiIo.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if ((imguiIo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
        {
            GLFWwindow* backup_current = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current);
        }
    }
} // namespace Nebula
