#include "Astrelis/Core/Result.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <variant>

using Astrelis::Result;

struct NonCopyable
{
    NonCopyable()                              = default;
    ~NonCopyable()                             = default;
    NonCopyable(const NonCopyable&)            = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&)                 = default;
    NonCopyable& operator=(NonCopyable&&)      = default;
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

TEST(ResultTest, ImplicitResultCreation)
{
    Result<int, float> result1 = 5;
    EXPECT_EQ(result1.IsOk(), true);

    Result<int, float> result2 = 5.0F;
    EXPECT_EQ(result2.IsErr(), true);

    Result<std::string, float> result3 = "Hello";
    EXPECT_EQ(result3.IsOk(), true);

    Result<std::string, float> result4 = 5.0F;
    EXPECT_EQ(result4.IsErr(), true);

    Result<NonCopyable, float> result7 = NonCopyable();
    EXPECT_EQ(result7.IsOk(), true);

    Result<float, NonCopyable> result8 = NonCopyable();
    EXPECT_EQ(result8.IsErr(), true);
}

TEST(ResultTest, ResultAccess)
{
    Result<int, float> result1 = 5;
    EXPECT_EQ(result1.IsOk(), true);
    EXPECT_EQ(result1.Unwrap(), 5);

    Result<int, float> result2 = 5.0F;
    EXPECT_EQ(result2.IsErr(), true);
    EXPECT_EQ(result2.UnwrapErr(), 5.0F);

    Result<std::string, float> result3 = "Hello";
    EXPECT_EQ(result3.IsOk(), true);
    EXPECT_EQ(result3.Unwrap(), "Hello");

    Result<std::string, float> result4 = 5.0F;
    EXPECT_EQ(result4.IsErr(), true);
    EXPECT_EQ(result4.UnwrapErr(), 5.0F);

    Result<float, float> result5 = Result<float, float>::Ok(5.0F);
    EXPECT_EQ(result5.IsOk(), true);
    EXPECT_EQ(result5.Unwrap(), 5.0F);

    Result<float, float> result6 = Result<float, float>::Err(5.0F);
    EXPECT_EQ(result6.IsErr(), true);
    EXPECT_EQ(result6.UnwrapErr(), 5.0F);
}

TEST(ResultTest, ResultDestruction)
{
    std::shared_ptr<std::monostate> destructionCounter = std::make_shared<std::monostate>();

    {
        Result<std::shared_ptr<std::monostate>, float> result1 = destructionCounter;
        EXPECT_EQ(result1.IsOk(), true);
        EXPECT_EQ(destructionCounter.use_count(), 2);
    }
    EXPECT_EQ(destructionCounter.use_count(), 1);

    {
        Result<float, std::shared_ptr<std::monostate>> result2 = destructionCounter;
        EXPECT_EQ(result2.IsErr(), true);
        EXPECT_EQ(destructionCounter.use_count(), 2);
    }
}

TEST(ResultTest, ResultAccessMove)
{
    Result<std::unique_ptr<int>, float> result1 = std::make_unique<int>(5);
    EXPECT_EQ(result1.IsOk(), true);
    {
        std::unique_ptr<int> value = std::move(result1.Unwrap());
        EXPECT_EQ(*value, 5);
    }
    // Check that the value was moved (pointer should be null)
    EXPECT_EQ(result1.Unwrap(), nullptr);

    Result<std::unique_ptr<int>, float> result2 = 5.0F;
    EXPECT_EQ(result2.IsErr(), true);
    EXPECT_EQ(result2.UnwrapErr(), 5.0F);

    Result<float, std::unique_ptr<int>> result3 = std::make_unique<int>(5);
    EXPECT_EQ(result3.IsErr(), true);
    EXPECT_EQ(*result3.UnwrapErr(), 5);

    Result<float, std::unique_ptr<int>> result4 = 5.0F;
    EXPECT_EQ(result4.IsOk(), true);
    EXPECT_EQ(result4.Unwrap(), 5.0F);
}

TEST(ResultTest, ResultAccessMoveNonCopyable)
{
    Result<std::unique_ptr<NonCopyable>, float> result1 = std::make_unique<NonCopyable>();
    EXPECT_EQ(result1.IsOk(), true);
    {
        std::unique_ptr<NonCopyable> value = std::move(result1.Unwrap());
        EXPECT_NE(value.get(), nullptr);
    }
    // Check that the value was moved (pointer should be null)
    EXPECT_EQ(result1.Unwrap(), nullptr);

    Result<NonCopyable, float> result2 = NonCopyable();
    EXPECT_EQ(result2.IsOk(), true);
    {
        NonCopyable value = std::move(result2.Unwrap());
    }
    // Nothing to test here, NonCopyable is a mono-state type
}

TEST(ResultTest, ResultExpectTest)
{
    Result<int, float> result1 = 5;
    EXPECT_EQ(result1.Expect("Error message"), 5);

    Result<int, float> result2 = 5.0F;
    EXPECT_THROW(result2.Expect("Error message"), std::runtime_error);

    Result<std::string, float> result3 = "Hello";
    EXPECT_EQ(result3.Expect("Error message"), "Hello");

    Result<std::string, float> result4 = 5.0F;
    EXPECT_THROW(result4.Expect("Error message"), std::runtime_error);

    Result<float, float> result5 = Result<float, float>::Ok(5.0F);
    EXPECT_EQ(result5.Expect("Error message"), 5.0F);

    Result<float, float> result6 = Result<float, float>::Err(5.0F);
    EXPECT_THROW(result6.Expect("Error message"), std::runtime_error);
}
