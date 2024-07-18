#include <gtest/gtest.h>

#include "NebulaEngine/Core/Result.hpp"

using Nebula::Result;

struct NonCopyable
{
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
};

TEST(ResultTest, ExplicitResultCreation)
{
    Result<int, float> result1 = Result<int, float>::Ok(5);
    EXPECT_EQ(result1.IsOk(), true);

    Result<int, float> result2 = Result<int, float>::Err(5.0F);
    EXPECT_EQ(result2.IsErr(), true);

    Result<float, float> result3 = Result<float, float>::Ok(5.0F);
    EXPECT_EQ(result3.IsOk(), true);

    Result<float, float> result4 = Result<float, float>::Err(5.0F);
    EXPECT_EQ(result4.IsErr(), true);

    Result<NonCopyable, NonCopyable> result7 = Result<NonCopyable, NonCopyable>::Ok(std::move(NonCopyable()));
    EXPECT_EQ(result7.IsOk(), true);

    Result<NonCopyable, NonCopyable> result8 = Result<NonCopyable, NonCopyable>::Err(std::move(NonCopyable()));
    EXPECT_EQ(result8.IsErr(), true);
}
