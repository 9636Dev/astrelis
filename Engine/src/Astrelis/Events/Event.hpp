#pragma once

#include <cstdint>
#include <string>

namespace Astrelis {
    enum class EventType : std::uint32_t {
        None = 0,
        WindowClosed,
        WindowResized,
        ViewportResized,
        WindowFocused,
        WindowLostFocus,
        WindowMoved,
        WindowMaximized,
        WindowMinimized,
        WindowRestored,
        WindowRefresh,
        WindowScale,
        AppRender,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
        MouseEntered,
        MouseLeft,
    };

    enum class EventCategory : std::uint32_t {
        None        = 0,
        Application = 1U << 0U,
        Input       = 1U << 1U,
        Keyboard    = 1U << 2U,
        Mouse       = 1U << 3U,
        MouseButton = 1U << 4U
    };

    inline constexpr std::uint32_t operator|(
        const EventCategory& lhs, const EventCategory& rhs) noexcept {
        return static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs);
    }

    inline constexpr std::uint32_t operator|(std::uint32_t lhs, const EventCategory& rhs) noexcept {
        return lhs | static_cast<std::uint32_t>(rhs);
    }

    class Event {
        friend class EventDispatcher;
    public:
        Event()                        = default;
        virtual ~Event()               = default;
        Event(const Event&)            = delete;
        Event& operator=(const Event&) = delete;
        Event(Event&&)                 = default;
        Event& operator=(Event&&)      = default;

        [[nodiscard]] virtual EventType     GetEventType() const noexcept     = 0;
        [[nodiscard]] virtual std::uint32_t GetCategoryFlags() const noexcept = 0;
        [[nodiscard]] virtual std::string   GetName() const                   = 0;
        [[nodiscard]] virtual std::string   ToString() const                  = 0;

        bool Handled = false;

        bool IsInCategory(const EventCategory& category) const noexcept {
            return (GetCategoryFlags() & static_cast<std::uint32_t>(category)) != 0U;
        }
    };

    class EventDispatcher {
    public:
        explicit EventDispatcher(Event& p_Event) noexcept : m_Event(p_Event) {
        }

        // F is derived by the compiler
        template<typename T, typename F> void Dispatch(const F& func) {
            if (m_Event.GetEventType() == T::GetStaticType()) {
                func(static_cast<T&>(m_Event));
            }
        }
    private:
        Event& m_Event; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    };

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_BIND_EVENT_FN(func) \
    [this](auto&&... args) -> decltype(auto) { \
        return this->func(std::forward<decltype(args)>(args)...); \
    }

} // namespace Astrelis
