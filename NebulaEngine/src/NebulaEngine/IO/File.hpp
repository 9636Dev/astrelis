#pragma once

#include <filesystem>
#include <utility>
#include <vector>

#include "NebulaEngine/Core/Result.hpp"

namespace Nebula
{
    class File
    {
    public:
        explicit File(const char* path) : m_Path(path) {}

        explicit File(std::string path) : m_Path(std::move(path)) {}

        explicit File(std::filesystem::path path) : m_Path(std::move(path)) {}

        bool Exists() const;
        bool IsDirectory() const;
        bool IsRegularFile() const;
        bool CanRead() const;
        bool CanWrite() const;
        bool CanExecute() const;

        const std::filesystem::path& GetPath() const;
        std::filesystem::path GetAbsolutePath() const;
        std::filesystem::path GetParentPath() const;
        std::filesystem::path GetRelativePath(const File& other) const;

        std::string GetFilename() const;
        std::string GetStem() const;
        std::string GetExtension() const;

        /**
        * @brief Check if the file can be read from, by checking if it exists and is a regular file, and has permissions to read.
        * @return True if the file can be read from, false otherwise.
        */
        bool CanReadFromFile() const;
        Result<std::string, std::string> ReadText(bool checking = true) const;
        Result<std::vector<char>, std::string> ReadBinary(bool checking = true) const;

        std::vector<File> ListFiles() const;
        File operator/(const File& other) const;

        bool operator==(const File& other) const noexcept;
        bool operator!=(const File& other) const noexcept;
    private:
        std::filesystem::path m_Path;
    };
} // namespace Nebula

