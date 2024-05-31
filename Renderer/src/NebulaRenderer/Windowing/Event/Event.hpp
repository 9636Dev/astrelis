#pragma once

#include <cstdint>
#include <string>

namespace Nebula
{
    enum class EventType : std::uint32_t
    {
        None = 0,
        WindowClosed,
        WindowResized,
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

    enum class EventCategory : std::uint32_t
    {
        None        = 0,
        Application = 1U << 0U,
        Input       = 1U << 1U,
        Keyboard    = 1U << 2U,
        Mouse       = 1U << 3U,
        MouseButton = 1U << 4U
    };

    inline constexpr std::uint32_t operator|(const EventCategory& lhs, const EventCategory& rhs) noexcept
    {
        return static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs);
    }

    inline constexpr std::uint32_t operator|(std::uint32_t lhs, const EventCategory& rhs) noexcept
    {
        return lhs | static_cast<std::uint32_t>(rhs);
    }

    class Event
    {
        friend class EventDispatcher;
    public:
        Event()                        = default;
        virtual ~Event()               = default;
        Event(const Event&)            = delete;
        Event& operator=(const Event&) = delete;
        Event(Event&&)                 = default;
        Event& operator=(Event&&)      = default;

        [[nodiscard]] virtual EventType GetEventType() const noexcept         = 0;
        [[nodiscard]] virtual std::uint32_t GetCategoryFlags() const noexcept = 0;
        [[nodiscard]] virtual std::string GetName() const                     = 0;
        [[nodiscard]] virtual std::string ToString() const                    = 0;
    private:
        bool m_Handled = false;
    };

    class EventDispatcher
    {
    public:
        explicit EventDispatcher(Event& p_Event) noexcept : m_Event(p_Event) {}

        // F is derived by the compiler
        template<typename T, typename F> bool Dispatch(const F& func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.m_Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }

        [[nodiscard]] bool IsHandled() const noexcept { return m_Event.m_Handled; }
    private:
        Event& m_Event; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    };
} // namespace Nebula
