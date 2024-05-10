#pragma once

#include "Event.hpp"
#include "KeyCodes.hpp"
#include <sstream>

namespace Nebula
{

    class KeyEvent : public Event
    {
    public:
        [[nodiscard]] KeyCode GetKeyCode() const noexcept { return m_KeyCode; }

        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override
        {
            return EventCategory::Input | EventCategory::Keyboard;
        }
    protected:
        explicit KeyEvent(const KeyCode keycode) noexcept : m_KeyCode(keycode) {}

        KeyCode
            m_KeyCode; // NOLINT(misc-non-private-member-variables-in-classes, cppcoreguidelines-non-private-member-variables-in-classes)
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        explicit KeyPressedEvent(KeyCode keycode, bool isRepeat = false) noexcept :
            KeyEvent(keycode),
            m_IsRepeat(isRepeat)
        {
        }

        [[nodiscard]] bool IsRepeat() const { return m_IsRepeat; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "KeyPressedEvent: " << static_cast<std::uint32_t>(m_KeyCode) << " (repeat = " << m_IsRepeat
                   << ")";
            return stream.str();
        }

        constexpr static EventType GetStaticType() noexcept { return EventType ::KeyPressed; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std::string GetName() const override { return "KeyPressed"; }
    private:
        bool m_IsRepeat;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(const KeyCode p_Keycode) : KeyEvent(p_Keycode) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "KeyReleasedEvent (Key: " << static_cast<std::uint16_t>(m_KeyCode) << ")";
            return stream.str();
        }

        static constexpr EventType GetStaticType() noexcept { return EventType::KeyReleased; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std::string GetName() const override { return "KeyReleased"; }
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(const KeyCode p_Keycode) : KeyEvent(p_Keycode) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "KeyTypedEvent (Key: " << static_cast<std::uint16_t>(m_KeyCode) << ")";
            return stream.str();
        }

        static constexpr EventType GetStaticType() noexcept { return EventType::KeyTyped; }

        [[nodiscard]] constexpr EventType GetEventType() const noexcept override { return GetStaticType(); }

        [[nodiscard]] std::string GetName() const override { return "KeyTyped"; }
    };
} // namespace Nebula
