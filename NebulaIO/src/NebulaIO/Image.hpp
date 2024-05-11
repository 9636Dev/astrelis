#pragma once

#include "Core.hpp"
#include "File.hpp"

#include <optional>
#include <vector>

namespace Nebula
{
    class NEBULA_IO_API InMemoryImage
    {
    public:
        static std::optional<InMemoryImage> LoadFromFile(const File& file);
        static std::optional<InMemoryImage> Load(const std::vector<std::byte>& bytes);

        ~InMemoryImage()                   = default;
        InMemoryImage(const InMemoryImage&) = default;
        InMemoryImage(InMemoryImage&&)      = default;
        InMemoryImage& operator=(const InMemoryImage&) = default;
        InMemoryImage& operator=(InMemoryImage&&) = default;

        [[nodiscard]] std::vector<std::byte> GetBytes() const { return m_Bytes; }
        [[nodiscard]] std::uint32_t GetWidth() const { return m_Width; }
        [[nodiscard]] std::uint32_t GetHeight() const { return m_Height; }
        [[nodiscard]] std::uint32_t GetChannels() const { return m_Channels; }
        [[nodiscard]] std::uint32_t GetBitsPerChannel() const { return m_BitsPerChannel; }
    private:
        InMemoryImage(std::vector<std::byte> bytes, std::uint32_t width, std::uint32_t height, std::uint32_t channels,
                      std::uint32_t bitsPerChannel)
            : m_Bytes(std::move(bytes)), m_Width(width), m_Height(height), m_Channels(channels),
              m_BitsPerChannel(bitsPerChannel)
        {
        }

        std::vector<std::byte> m_Bytes;
        std::uint32_t m_Width;
        std::uint32_t m_Height;
        std::uint32_t m_Channels;
        std::uint32_t m_BitsPerChannel;
    };
} // namespace Nebula
