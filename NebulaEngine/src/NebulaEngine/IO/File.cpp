#include "File.hpp"

#include <fstream>
#include <sstream>

namespace Nebula
{
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
} // namespace Nebula
