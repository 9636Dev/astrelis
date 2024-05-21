#pragma once

#include "Event.hpp"

#include <sstream>

namespace Nebula
{
    class WindowMovedEvent : public Event
    {
    public:
        WindowMovedEvent(std::uint32_t width, std::uint32_t height) : m_PosX(width), m_PosY(height) {}

        [[nodiscard]] std::uint32_t GetPosX() const noexcept { return m_PosX; }

        [[nodiscard]] std::uint32_t GetPosY() const noexcept { return m_PosY; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "WindowMoveEvent (" << m_PosX << ", " << m_PosY << ")";
            return stream.str();
        }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowMoved; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowMove"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    private:
        std::uint32_t m_PosX;
        std::uint32_t m_PosY;
    };

    class WindowResizedEvent : public Event
    {
    public:
        WindowResizedEvent(std::uint32_t width, std::uint32_t height) : m_Width(width), m_Height(height) {}

        [[nodiscard]] std::uint32_t GetWidth() const noexcept { return m_Width; }

        [[nodiscard]] std::uint32_t GetHeight() const noexcept { return m_Height; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "WindowResizeEvent (" << m_Width << ", " << m_Height << ")";
            return stream.str();
        }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowResized; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowResize"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    private:
        std::uint32_t m_Width;
        std::uint32_t m_Height;
    };

    class WindowFocusedEvent : public Event
    {
    public:
        WindowFocusedEvent() = default;

        [[nodiscard]] inline std::string ToString() const override { return "WindowFocusEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowFocused; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowFocus"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    };

    class WindowLostFocusEvent : public Event
    {
    public:
        WindowLostFocusEvent() = default;

        [[nodiscard]] inline std::string ToString() const override { return "WindowLostFocusEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowLostFocus; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowLostFocus"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    };

    class WindowClosedEvent : public Event
    {
    public:
        WindowClosedEvent() = default;

        [[nodiscard]] inline std::string ToString() const override { return "WindowCloseEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowClosed; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowClose"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    };

    class WindowMaximizedEvent : public Event
    {
    public:
        WindowMaximizedEvent() = default;

        [[nodiscard]] inline std::string ToString() const override { return "WindowMaximizedEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowMaximized; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowMaximized"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    };

    class WindowMinimizedEvent : public Event
    {
    public:
        WindowMinimizedEvent() = default;

        [[nodiscard]] inline std::string ToString() const override { return "WindowMinimizedEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowMinimized; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowMinimized"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    };

    class WindowRestoredEvent : public Event
    {
    public:
        WindowRestoredEvent() = default;

        [[nodiscard]] inline std::string ToString() const override { return "WindowRestoredEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowRestored; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowRestored"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    };

    class WindowRefreshEvent : public Event
    {
    public:
        WindowRefreshEvent() = default;

        [[nodiscard]] inline std::string ToString() const override { return "WindowRefreshEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowRefresh; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowRefresh"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    };

    class WindowScaleEvent : public Event
    {
    public:
        WindowScaleEvent(float scaleX, float scaleY) : m_ScaleX(scaleX), m_ScaleY(scaleY) {}

        [[nodiscard]] float GetScaleX() const noexcept { return m_ScaleX; }

        [[nodiscard]] float GetScaleY() const noexcept { return m_ScaleY; }

        [[nodiscard]] inline std::string ToString() const override
        {
            std::stringstream stream;
            stream << "WindowScaleEvent (" << m_ScaleX << ", " << m_ScaleY << ")";
            return stream.str();
        }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowScale; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowScale"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    private:
        float m_ScaleX;
        float m_ScaleY;
    };
} // namespace Nebula
