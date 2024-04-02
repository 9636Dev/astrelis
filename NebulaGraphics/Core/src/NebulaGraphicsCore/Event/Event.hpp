#pragma once

#include "NebulaGraphicsCore/Core.hpp"

#include <string>

#define EVENT_CLASS_TYPE(type)                                                             \
    static constexpr EventType GetStaticType() noexcept { return EventType::type; }        \
    constexpr EventType GetEventType() const noexcept override { return GetStaticType(); } \
    std::string GetName() const override { return #type; }

namespace Nebula
{
    enum class EventType : std::uint32_t
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
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

    class NEBULA_GRAPHICS_CORE_API Event
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
