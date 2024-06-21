#pragma once

#include <cstddef>
#include <vector>

#include "File.hpp"

namespace Nebula
{
    class InMemoryImage
    {
    public:
        explicit InMemoryImage(const File& file);
        ~InMemoryImage() = default;
        InMemoryImage(const InMemoryImage&);
        InMemoryImage& operator=(const InMemoryImage&);
        InMemoryImage(InMemoryImage&&) noexcept;
        InMemoryImage& operator=(InMemoryImage&&) noexcept;

        [[nodiscard]] const std::vector<std::byte>& GetData() const { return m_Data; }

        [[nodiscard]] int GetWidth() const { return m_Width; }

        [[nodiscard]] int GetHeight() const { return m_Height; }

        [[nodiscard]] int GetChannels() const { return m_Channels; }
    private:
        std::vector<std::byte> m_Data;
        int m_Width;
        int m_Height;
        int m_Channels;
    };
} // namespace Nebula
