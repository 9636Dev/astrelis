#pragma once

#include "NebulaEngine/Core/Result.hpp"

#include <filesystem>
#include <utility>
#include <vector>

namespace Nebula
{
    class File
    {
    public:
        explicit File(const char* path) : m_Path(path) {}

        explicit File(std::string path) : m_Path(std::move(path)) {}

        explicit File(std::filesystem::path path) : m_Path(std::move(path)) {}

        bool Exists() const { return std::filesystem::exists(m_Path); }

        bool IsDirectory() const { return std::filesystem::is_directory(m_Path); }

        bool IsRegularFile() const { return std::filesystem::is_regular_file(m_Path); }

        bool CanRead() const
        {
            return (std::filesystem::perms::owner_read & std::filesystem::status(m_Path).permissions()) !=
                   std::filesystem::perms::none;
        }

        bool CanWrite() const
        {
            return (std::filesystem::perms::owner_write & std::filesystem::status(m_Path).permissions()) !=
                   std::filesystem::perms::none;
        }

        bool CanExecute() const
        {
            return (std::filesystem::perms::owner_exec & std::filesystem::status(m_Path).permissions()) !=
                   std::filesystem::perms::none;
        }

        std::filesystem::path GetPath() const { return m_Path; }

        std::filesystem::path GetAbsolutePath() const { return std::filesystem::absolute(m_Path); }

        std::filesystem::path GetParentPath() const { return m_Path.parent_path(); }

        std::filesystem::path GetRelativePath(const File& other) const
        {
            return std::filesystem::relative(m_Path, other.GetPath());
        }

        std::string GetFilename() const { return m_Path.filename().string(); }

        std::string GetStem() const { return m_Path.stem().string(); }

        std::string GetExtension() const { return m_Path.extension().string(); }

        Result<std::string, std::string> ReadText(bool checking = true) const;
        Result<std::vector<char>, std::string> ReadBinary(bool checking = true) const;

        std::vector<File> ListFiles() const;

        bool operator==(const File& other) const
        {
            return m_Path == other.m_Path;
        }
    private:
        std::filesystem::path m_Path;
    };
} // namespace Nebula

