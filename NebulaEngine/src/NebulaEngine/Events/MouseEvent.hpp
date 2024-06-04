#pragma once

#include "Event.hpp"

#include <cstdint>
#include <sstream>

namespace Nebula
{
    enum class MouseCode : std::uint16_t
    {
        Button0      = 0,
        Button1      = 1,
        Button2      = 2,
        Button3      = 3,
        Button4      = 4,
        Button5      = 5,
        Button6      = 6,
        Button7      = 7,
        ButtonLast   = Button7,
        ButtonLeft   = Button0,
        ButtonRight  = Button1,
        ButtonMiddle = Button2
    };

    class MouseEnteredEvent : public Event
    {
    public:
        MouseEnteredEvent() = default;

        [[nodiscard]] std::string ToString() const override { return "MouseEnteredEvent"; }

        static constexpr EventType GetStaticType() noexcept { return EventType::MouseEntered; }

        [[nodiscard]] EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std::string GetName() const override { return "MouseEntered"; }

        [[nodiscard]] std::uint32_t GetCategoryFlags() const noexcept override
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

        [[nodiscard]] EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std::string GetName() const override { return "MouseLeft"; }

        [[nodiscard]] std::uint32_t GetCategoryFlags() const noexcept override
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

        [[nodiscard]] EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "MouseMoved"; }

        [[nodiscard]] std::uint32_t GetCategoryFlags() const noexcept override
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

        [[nodiscard]] EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std::string GetName() const override { return "MouseScrolled"; }

        [[nodiscard]] std::uint32_t GetCategoryFlags() const noexcept override
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

        [[nodiscard]] std::uint32_t GetCategoryFlags() const noexcept override
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

        [[nodiscard]] EventType GetEventType() const noexcept override { return GetStaticType(); }

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

        [[nodiscard]] EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std ::string GetName() const override { return "MouseButtonReleased"; }
    };
} // namespace Nebula
