#include "Window.hpp"

#include "PlatformDetection.hpp"

#ifdef NEBULA_PLATFORM_LINUX
    #include "Platform/Linux/LinuxWindow.hpp"
#elif defined(NEBULA_PLATFORM_MACOS)
    #include "Platform/MacOS/MacOSWindow.hpp"
#elif defined(NEBULA_PLATFORM_WINDOWS)
    #include "Platform/Windows/WindowsWindow.hpp"
#endif

namespace Nebula
{
    Result<RefPtr<Window>, std::string> Window::Create(const WindowProps& props)
    {
#ifdef NEBULA_PLATFORM_LINUX
        return LinuxWindow::Create(props).MapMove(
            [](RefPtr<LinuxWindow>&& window) { return static_cast<RefPtr<Window>>(window); });
#elif defined(NEBULA_PLATFORM_MACOS)
        return MacOSWindow::Create(props).MapMove(
            [](RefPtr<MacOSWindow>&& window) { return static_cast<RefPtr<Window>>(window); });
#elif defined(NEBULA_PLATFORM_WINDOWS)
        return WindowsWindow::Create(props).MapMove(
            [](RefPtr<WindowsWindow>&& window) { return static_cast<RefPtr<Window>>(window); });
#else
        return "Unsupported platform";
#endif
    }
} // namespace Nebula
