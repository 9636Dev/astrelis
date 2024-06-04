#include "Nebulapch.hpp"
#include "Application.hpp"

#include <utility>

namespace Nebula
{
    Application* Application::s_Instance = nullptr;

    Application::Application(ApplicationSpecification specification)
        : m_Specification(std::move(specification))
    {
        NEBULA_VERIFY(s_Instance == nullptr, "Application already exists (Should be singleton)");
        s_Instance = this;
    }

    Application::~Application() = default;

    void Application::Run()
    {
    }
} // namespace Nebula
