#pragma once
#include "Event.hpp"

#include <sstream>

namespace Nebula {

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(std::uint32_t width, std::uint32_t height)
            : m_Width(width), m_Height(height) {}

        [[nodiscard]] std::uint32_t GetWidth() const noexcept { return m_Width; }
        [[nodiscard]] std::uint32_t GetHeight() const noexcept { return m_Height; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream stream;
            stream << "WindowResizeEvent (" << m_Width << ", " << m_Height << ")";
            return stream.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override { return static_cast<std::uint32_t>(EventCategory::Application); }
    private:
        std::uint32_t m_Width;
        std::uint32_t m_Height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        [[nodiscard]] inline std::string ToString() const override { return "WindowCloseEvent"; }

        EVENT_CLASS_TYPE(WindowClose)
        [[nodiscard]] constexpr std::uint32_t GetCategoryFlags() const noexcept override { return static_cast<std::uint32_t>(EventCategory::Application); }
    };
} // namespace Nebula
