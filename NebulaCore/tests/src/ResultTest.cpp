#include "NebulaCore/Util/Result.hpp"
#include <gtest/gtest.h>

TEST(ResultTest, TestResult)
{
    Nebula::Result<int, std::string> result = Nebula::Result<int, std::string>::Ok(5);
    EXPECT_TRUE(result.IsOk());
    EXPECT_FALSE(result.IsErr());
    EXPECT_EQ(result.Unwrap(), 5);

    Nebula::Result<int, std::string> result2 = Nebula::Result<int, std::string>::Err("Error");
    EXPECT_FALSE(result2.IsOk());
    EXPECT_TRUE(result2.IsErr());
    EXPECT_EQ(result2.UnwrapErr(), "Error");
}

TEST(ResultTest, TestMap)
{
    Nebula::Result<int, std::string> result = Nebula::Result<int, std::string>::Ok(5);
    auto mappedResult = result.Map<float>([](int value) { return static_cast<float>(value) * 2.0F; });
    EXPECT_TRUE(mappedResult.IsOk());
    EXPECT_EQ(mappedResult.Unwrap(), 10.0F);

    Nebula::Result<int, std::string> result2 = Nebula::Result<int, std::string>::Err("Error");
    auto mappedResult2 = result2.Map<float>([](int value) { return static_cast<float>(value) * 2.0F; });
    EXPECT_FALSE(mappedResult2.IsOk());
    EXPECT_TRUE(mappedResult2.IsErr());
    EXPECT_EQ(mappedResult2.UnwrapErr(), "Error");
}
