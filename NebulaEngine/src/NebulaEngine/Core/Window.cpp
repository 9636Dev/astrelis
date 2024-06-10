#include "Window.hpp"

#include "PlatformDetection.hpp"

#ifdef NEBULA_PLATFORM_LINUX
    #include "NebulaEngine/Platform/Linux/LinuxWindow.hpp"
#elif defined(NEBULA_PLATFORM_MACOS)
    #include "NebulaEngine/Platform/MacOS/MacOSWindow.hpp"
#endif

namespace Nebula
{
    Result<Ptr<Window>, std::string> Window::Create(const WindowProps& props)
    {
#ifdef NEBULA_PLATFORM_LINUX
        return LinuxWindow::Create(props).MapMove([](Ptr<LinuxWindow>&& window) { return window.Cast<Window>(); });
#elif defined(NEBULA_PLATFORM_MACOS)
        return MacOSWindow::Create(props).MapMove([](Ptr<MacOSWindow>&& window) { return window.Cast<Window>(); });
#else
        return "Unsupported platform";
#endif
    }
} // namespace Nebula
