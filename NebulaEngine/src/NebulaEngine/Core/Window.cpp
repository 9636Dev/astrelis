#include "Window.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "PlatformDetection.hpp"

#ifdef NEBULA_PLATFORM_LINUX
    #include "Platform/Linux/LinuxWindow.hpp"
#elif defined(NEBULA_PLATFORM_MACOS)
    #include "Platform/MacOS/MacOSWindow.hpp"
#endif

namespace Nebula
{
    Result<RefPtr<Window>, std::string> Window::Create(const WindowProps& props)
    {
#ifdef NEBULA_PLATFORM_LINUX
        return LinuxWindow::Create(props).MapMove([](Ptr<LinuxWindow>&& window) { return window.Cast<Window>(); });
#elif defined(NEBULA_PLATFORM_MACOS)
        auto window = MacOSWindow::Create(props);
        return window.MapMove([](RefPtr<MacOSWindow>&& window) { return static_cast<RefPtr<Window>>(window); });
#else
        return "Unsupported platform";
#endif
    }
} // namespace Nebula
