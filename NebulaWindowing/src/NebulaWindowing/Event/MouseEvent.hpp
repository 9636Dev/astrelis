#pragma once

#include "Event.hpp"
#include "MouseCodes.hpp"

#include <sstream>

namespace Nebula
{
    class MouseEnteredEvent : public Event
    {
    public:
        MouseEnteredEvent() = default;

        [[nodiscard]] std::string ToString() const override { return "MouseEnteredEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType::MouseEntered; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std::string GetName() const override { return "MouseEntered"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return EventCategory::Mouse | EventCategory::Input;
        }
    };

    class MouseLeftEvent : public Event
    {
    public:
        MouseLeftEvent() = default;

        [[nodiscard]] std::string ToString() const override { return "MouseLeftEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType::MouseLeft; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std::string GetName() const override { return "MouseLeft"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return EventCategory::Mouse | EventCategory::Input;
        }
    };

    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float p_MouseX, float p_MouseY) : m_MouseX(p_MouseX), m_mouseY(p_MouseY) {}

        [[nodiscard]] float GetMouseX() const noexcept { return m_MouseX; }

        [[nodiscard]] float GetMouseY() const noexcept { return m_mouseY; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "MouseMovedEvent (Pos: " << m_MouseX << ", " << m_mouseY << ")";
            return stream.str();
        }

        static constexpr EventType GetStaticType() noexcept { return EventType ::MouseMoved; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "MouseMoved"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return EventCategory::Mouse | EventCategory::Input;
        }
    private:
        float m_MouseX;
        float m_mouseY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(const float p_OffsetX, const float p_OffsetY) : m_OffsetX(p_OffsetX), m_OffsetY(p_OffsetY) {}

        [[nodiscard]] float GetOffsetX() const noexcept { return m_OffsetX; }

        [[nodiscard]] float GetOffsetY() const noexcept { return m_OffsetY; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "MouseScrolledEvent (Offset: " << m_OffsetX << ", " << m_OffsetY << ")";
            return stream.str();
        }

        static constexpr EventType GetStaticType() noexcept { return EventType::MouseScrolled; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std::string GetName() const override { return "MouseScrolled"; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return EventCategory::Mouse | EventCategory::Input;
        }
    private:
        float m_OffsetX;
        float m_OffsetY;
    };

    class MouseButtonEvent : public Event
    {
    public:
        [[nodiscard]] MouseCode GetMouseButton() const noexcept { return m_Button; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return EventCategory::Mouse | EventCategory::Input | EventCategory::MouseButton;
        }
    protected:
        explicit MouseButtonEvent(MouseCode button) : m_Button(button) {}

        MouseCode m_Button; // NOLINT(*-non-private-member-variables-in-classes)
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) {};

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "MouseButtonPressedEvent (Button: " << static_cast<std::uint16_t>(m_Button) << ")";
            return stream.str();
        }

        static constexpr EventType GetStaticType() noexcept { return EventType ::MouseButtonPressed; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std::string GetName() const override { return "MouseButtonPressed"; }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "MouseButtonReleasedEvent (Button:" << static_cast<std::uint16_t>(m_Button) << ")";
            return stream.str();
        }

        static constexpr EventType GetStaticType() noexcept { return EventType ::MouseButtonReleased; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "MouseButtonReleased"; }
    };
} // namespace Nebula
