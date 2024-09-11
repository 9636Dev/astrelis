#include "Window.hpp"
#include "Astrelis/Core/Base.hpp"

#ifdef ASTRELIS_PLATFORM_LINUX
    #include "Platform/Linux/LinuxWindow.hpp"
#elif defined(ASTRELIS_PLATFORM_MACOS)
    #include "Platform/MacOS/MacOSWindow.hpp"
#elif defined(ASTRELIS_PLATFORM_WINDOWS)
    #include "Platform/Windows/WindowsWindow.hpp"
#endif

namespace Astrelis
{
    Result<RefPtr<Window>, std::string> Window::Create(const WindowProps& props)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::Window::Create");
#ifdef ASTRELIS_PLATFORM_LINUX
        return LinuxWindow::Create(props).MapMove(
            [](RefPtr<LinuxWindow>&& window) { return static_cast<RefPtr<Window>>(window); });
#elif defined(ASTRELIS_PLATFORM_MACOS)
        return MacOSWindow::Create(props).MapMove(
            [](RefPtr<MacOSWindow>&& window) { return static_cast<RefPtr<Window>>(window); });
#elif defined(ASTRELIS_PLATFORM_WINDOWS)
        return WindowsWindow::Create(props).MapMove(
            [](RefPtr<WindowsWindow>&& window) { return static_cast<RefPtr<Window>>(window); });
#else
        return "Unsupported platform";
#endif
    }
} // namespace Astrelis
