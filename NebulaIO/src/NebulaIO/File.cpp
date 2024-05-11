#include "File.hpp"

#include <fstream>
#include <utility>

namespace Nebula
{
    File::File(std::filesystem::path path) : m_Path(std::move(path)) {}

    std::string File::GetAbsolutePath() const { return std::filesystem::absolute(m_Path).string(); }

    std::pair<bool, std::string> File::ReadText() const
    {
        std::ifstream fileStream(m_Path);
        if (!fileStream.is_open())
        {
            return {false, ""};
        }

        std::string text((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
        return {true, std::move(text)};
    }

    std::pair<bool, std::vector<std::byte>> File::ReadBytes() const
    {
        std::ifstream fileStream(m_Path, std::ios::binary | std::ios::ate);
        if (!fileStream.is_open())
        {
            return {};
        }

        std::streamsize size = fileStream.tellg();
        if (size == -1)
        {
            return {false, {}};
        }

        fileStream.seekg(0, std::ios::beg);

        std::vector<std::byte> bytes(static_cast<std::size_t>(size));
        fileStream.read(/* NOLINT(cppcoreguidelines-pro-type-reinterpret-cast) */ reinterpret_cast<char*>(bytes.data()), size);

        return {true, std::move(bytes)};
    }
} // namespace Nebula
