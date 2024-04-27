#include "File.hpp"

#include <utility>

namespace Nebula
{
    File::File(std::filesystem::path path) : m_Path(std::move(path)) {}

    std::string File::GetAbsolutePath() const { return std::filesystem::absolute(m_Path).string(); }
} // namespace Nebula
