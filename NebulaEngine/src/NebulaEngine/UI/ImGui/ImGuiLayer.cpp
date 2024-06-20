#include "ImGuiLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/PlatformDetection.hpp"
#include "NebulaEngine/Events/Event.hpp"
#include "NebulaEngine/UI/ImGui/ImGuiBackend.hpp"

#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Nebula
{
    ImGuiLayer::ImGuiLayer(RefPtr<ImGuiBackend> backend) : Layer("ImGuiLayer"), m_Backend(std::move(backend)) {}

    void ImGuiLayer::OnAttach()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& imguiIO = ImGui::GetIO();
        imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        imguiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
#ifdef NEBULA_PLATFORM_LINUX
        NEBULA_CORE_LOG_DEBUG("ImGui Multi-Viewport not supported on Linux (Wayland)");
#else
        imguiIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
#endif

        if ((imguiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
        {
            ImGuiStyle& style                 = ImGui::GetStyle();
            style.WindowRounding              = 0.0F;
            style.Colors[ImGuiCol_WindowBg].w = 1.0F;
        }

        if (m_Backend != nullptr)
        {
            m_Backend->Init();
        }

        // Set Dark Theme Colors
        SetDarkThemeColors();
    }

    void ImGuiLayer::OnDetach()
    {
        m_Backend->Shutdown();
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

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<ViewportResizedEvent>(NEBULA_BIND_EVENT_FN(Resize));
    }

    void ImGuiLayer::Resize(ViewportResizedEvent& event)
    {
        m_Backend->Resize(static_cast<std::int32_t>(event.GetWidth()), static_cast<std::int32_t>(event.GetHeight()));
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void ImGuiLayer::Begin()
    {
        m_Backend->Begin();
        ImGui::NewFrame();
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void ImGuiLayer::End()
    {
        ImGuiIO& imguiIo    = ImGui::GetIO();

        // Rendering
        ImGui::Render();
        m_Backend->End();

        if ((imguiIo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
        {
            GLFWwindow* backup_current = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current);
        }
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void ImGuiLayer::SetDarkThemeColors() { ImGui::StyleColorsDark(); }
} // namespace Nebula
