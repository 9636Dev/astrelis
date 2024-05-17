#include "NebulaCore/Profile/Profile.hpp"
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>


TEST(ProfilingTest, ProfileTest)
{
    auto tempDir = std::filesystem::temp_directory_path();
    auto tempFile = (tempDir / "test.prof.json").string();

    NEBULA_PROFILE_BEGIN_SESSION("StartupProfile", tempFile);
    NEBULA_PROFILE_END_SESSION();

    ASSERT_TRUE(std::filesystem::exists(tempFile));

    std::ifstream file(tempFile);
    ASSERT_TRUE(file.is_open());
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    ASSERT_EQ(buffer.str(), "{}");

    std::filesystem::remove(tempFile);
}
