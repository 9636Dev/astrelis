#pragma once

#include <cstddef>
#include <vector>

#include "File.hpp"

namespace Astrelis
{
    class InMemoryImage
    {
    public:
        explicit InMemoryImage(const File& file);

        InMemoryImage(int width, int height, int channels, const std::vector<std::byte>& data) :
            m_Data(data),
            m_Width(width),
            m_Height(height),
            m_Channels(channels)
        {
        }

        ~InMemoryImage()                                   = default;
        InMemoryImage(const InMemoryImage&)                = default;
        InMemoryImage& operator=(const InMemoryImage&)     = default;
        InMemoryImage(InMemoryImage&&) noexcept            = default;
        InMemoryImage& operator=(InMemoryImage&&) noexcept = default;

        [[nodiscard]] const std::vector<std::byte>& GetData() const { return m_Data; }

        [[nodiscard]] int GetWidth() const { return m_Width; }

        [[nodiscard]] int GetHeight() const { return m_Height; }

        [[nodiscard]] int GetChannels() const { return m_Channels; }

        bool Save(const std::string& filename) const;

        enum class CompareMethod
        {
            SSIM,
        };

        float Similarity(const InMemoryImage& other, CompareMethod method) const;

        bool operator==(const InMemoryImage& other) const
        {
            return m_Data == other.m_Data && m_Width == other.m_Width && m_Height == other.m_Height &&
                   m_Channels == other.m_Channels;
        }
    private:
        std::vector<std::byte> m_Data;
        int m_Width;
        int m_Height;
        int m_Channels;
    };
} // namespace Astrelis
