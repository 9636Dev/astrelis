#pragma once

#include "Astrelis/Core/Result.hpp"
#include "Astrelis/Core/Window.hpp"
#include "Astrelis/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Astrelis {
    struct MacOSWindowData final : public BaseWindowData {
        explicit MacOSWindowData(std::string title, Dimension2Du dimensions)
            : BaseWindowData(std::move(title), dimensions) {
        }
    };

    class MacOSWindow final : public Window {
    public:
        explicit MacOSWindow(GLFWwindow* window, MacOSWindowData data);
        ~MacOSWindow() final;
        MacOSWindow(const MacOSWindow&)            = delete;
        MacOSWindow& operator=(const MacOSWindow&) = delete;
        MacOSWindow(MacOSWindow&&)                 = delete;
        MacOSWindow& operator=(MacOSWindow&&)      = delete;

        void BeginFrame() final;
        void EndFrame() final;
        void OnUpdate() final;
        void WaitForEvents() final;

        void SetEventCallback(const WindowEventCallback& callback) final {
            m_Data.EventCallback = callback;
        }

        RefPtr<GraphicsContext> GetGraphicsContext() const final {
            return m_Context;
        }

        Rect2Di GetViewportBounds() const final;

        inline std::uint32_t GetWidth() const final {
            return m_Data.Dimensions.Width;
        }

        inline std::uint32_t GetHeight() const final {
            return m_Data.Dimensions.Height;
        }

        inline void* GetNativeWindow() const final {
            return m_Window.Get();
        }

        inline void SetVSync(bool enabled) final {
            m_Context->SetVSync(enabled);
        }

        inline bool IsVSync() const final {
            return m_Context->IsVSync();
        }

        static Result<RefPtr<MacOSWindow>, std::string> Create(const WindowProps& props);
    private:
        OwnedPtr<GLFWwindow*>   m_Window;
        MacOSWindowData         m_Data;
        RefPtr<GraphicsContext> m_Context;
    };
} // namespace Astrelis
