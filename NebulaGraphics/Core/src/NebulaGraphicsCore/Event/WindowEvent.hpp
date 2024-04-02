#pragma once
#include "Event.hpp"

#include <sstream>

namespace Nebula
{
    class WindowMoveEvent : public Event
    {
    public:
        WindowMoveEvent(std::uint32_t width, std::uint32_t height) : m_PosX(width), m_PosY(height) {}

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

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(std::uint32_t width, std::uint32_t height) : m_Width(width), m_Height(height) {}

        [[nodiscard]] std::uint32_t GetWidth() const noexcept { return m_Width; }

        [[nodiscard]] std::uint32_t GetHeight() const noexcept { return m_Height; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "WindowResizeEvent (" << m_Width << ", " << m_Height << ")";
            return stream.str();
        }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowResize; }

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

    class WindowFocusEvent : public Event
    {
    public:
        WindowFocusEvent() = default;

        [[nodiscard]] inline std::string ToString() const override { return "WindowFocusEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowFocus; }

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

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        [[nodiscard]] inline std::string ToString() const override { return "WindowCloseEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType ::WindowClose; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "WindowClose"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return static_cast<std::uint32_t>(EventCategory::Application);
        }
    };
} // namespace Nebula
