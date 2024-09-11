#include "File.hpp"
#include "Astrelis/Core/Base.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace Astrelis
{
    bool File::Exists() const { return std::filesystem::exists(m_Path); }

    bool File::IsDirectory() const { return std::filesystem::is_directory(m_Path); }

    bool File::IsRegularFile() const { return std::filesystem::is_regular_file(m_Path); }

    bool File::CanRead() const
    {
        return (std::filesystem::perms::owner_read & std::filesystem::status(m_Path).permissions()) !=
               std::filesystem::perms::none;
    }

    bool File::CanWrite() const
    {
        return (std::filesystem::perms::owner_write & std::filesystem::status(m_Path).permissions()) !=
               std::filesystem::perms::none;
    }

    bool File::CanExecute() const
    {
        return (std::filesystem::perms::owner_exec & std::filesystem::status(m_Path).permissions()) !=
               std::filesystem::perms::none;
    }

    const std::filesystem::path& File::GetPath() const { return m_Path; }

    std::filesystem::path File::GetAbsolutePath() const { return std::filesystem::absolute(m_Path); }

    std::filesystem::path File::GetParentPath() const { return m_Path.parent_path(); }

    std::filesystem::path File::GetRelativePath(const File& other) const
    {
        return std::filesystem::relative(m_Path, other.GetPath());
    }

    std::string File::GetFilename() const { return m_Path.filename().string(); }

    std::string File::GetStem() const { return m_Path.stem().string(); }

    std::string File::GetExtension() const { return m_Path.extension().string(); }

    bool File::CanReadFromFile() const { return Exists() && IsRegularFile() && CanRead(); }

    Result<std::string, std::string> File::ReadText(bool checking) const
    {
        if (checking)
        {
            if (!Exists())
            {
                return Result<std::string, std::string>::Err("File does not exist");
            }

            if (!IsRegularFile())
            {
                return Result<std::string, std::string>::Err("File is not a regular file");
            }

            if (!CanRead())
            {
                return Result<std::string, std::string>::Err("File cannot be read");
            }
        }

        std::ifstream fileStream(m_Path);
        if (!fileStream.is_open())
        {
            return Result<std::string, std::string>::Err("Failed to open file");
        }

        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        return Result<std::string, std::string>::Ok(buffer.str());
    }

    Result<std::vector<char>, std::string> File::ReadBinary(bool checking) const
    {
        if (checking)
        {
            if (!Exists())
            {
                return "File does not exist";
            }

            if (!IsRegularFile())
            {
                return "File is not a regular file";
            }

            if (!CanRead())
            {
                return "File cannot be read";
            }
        }

        std::ifstream fileStream(m_Path, std::ios::binary);
        if (!fileStream.is_open())
        {
            return "Failed to open file";
        }

        std::vector<char> buffer((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
        return buffer;
    }

    std::vector<File> File::ListFiles() const
    {
        std::vector<File> files;
        std::filesystem::directory_iterator iter(m_Path);
        for (const auto& path : iter)
        {
            files.emplace_back(path.path());
        }

        return files;
    }

    File File::operator/(const File& other) const { return File(m_Path / other.GetPath()); }

    bool File::operator==(const File& other) const noexcept { return m_Path == other.m_Path; }

    bool File::operator!=(const File& other) const noexcept { return m_Path != other.m_Path; }
} // namespace Astrelis
