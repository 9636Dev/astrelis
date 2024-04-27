#pragma once

#include "Core.hpp"

#include <filesystem>
#include <string>

namespace Nebula
{
    class NEBULA_IO_API File
    {
    public:
        explicit File(std::filesystem::path path);

        static File FromPathString(const std::string& path) { return File(std::filesystem::path(path)); }

        ~File()                      = default;
        File(const File&)            = default;
        File(File&&)                 = default;
        File& operator=(const File&) = default;
        File& operator=(File&&)      = default;

        [[nodiscard]] bool Exists() const { return std::filesystem::exists(m_Path); }

        [[nodiscard]] std::filesystem::path GetPath() const { return m_Path; }

        [[nodiscard]] std::string GetExtension() const { return m_Path.extension().string(); }

        [[nodiscard]] std::string GetFileName() const { return m_Path.filename().string(); }

        [[nodiscard]] std::string GetFileNameWithoutExtension() const { return m_Path.stem().string(); }

        [[nodiscard]] std::filesystem::path GetParentPath() const { return m_Path.parent_path(); }

        [[nodiscard]] std::string GetAbsolutePath() const;
    private:
        std::filesystem::path m_Path;
    };

    struct FileWriteResult
    {
        bool Success;
        std::size_t BytesWritten;

        FileWriteResult(bool success, std::size_t bytesWritten) : Success(success), BytesWritten(bytesWritten) {}
    };
} // namespace Nebula
