#include "NebulaCore/Util/Result.hpp"
#include <gtest/gtest.h>

struct NonCopyable
{
    NonCopyable()                              = default;
    ~NonCopyable()                             = default;
    NonCopyable(const NonCopyable&)            = delete;
    NonCopyable(NonCopyable&&)                 = default;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&&)      = default;
};

struct NonMoveable
{
    NonMoveable()                              = default;
    ~NonMoveable()                             = default;
    NonMoveable(const NonMoveable&)            = default;
    NonMoveable(NonMoveable&&)                 = delete;
    NonMoveable& operator=(const NonMoveable&) = default;
    NonMoveable& operator=(NonMoveable&&)      = delete;
};

TEST(ResultTest, TestResult)
{
    Nebula::Result<int, double> res(5);
    EXPECT_TRUE(res.IsOk());
    EXPECT_EQ(res.Unwrap(), 5);

    Nebula::Result<int, double> res2(5.0);
    EXPECT_TRUE(res2.IsErr());
    EXPECT_EQ(res2.UnwrapErr(), 5.0);

    Nebula::Result<int, int> res3(5);
    EXPECT_TRUE(res3.IsOk());
    EXPECT_EQ(res3.Unwrap(), 5);

    Nebula::Result<int, int> res4(5, false);
    EXPECT_TRUE(res4.IsErr());
    EXPECT_EQ(res4.UnwrapErr(), 5);

    Nebula::Result<NonCopyable, int> res5(std::move(NonCopyable()));
    EXPECT_TRUE(res5.IsOk());
    EXPECT_NO_THROW((void)res5.Unwrap());

    NonCopyable nco;
    Nebula::Result<int, NonCopyable> res6(std::move(nco));
    EXPECT_TRUE(res6.IsErr());
    EXPECT_NO_THROW((void)res6.UnwrapErr());

    NonMoveable nmo;
    Nebula::Result<NonMoveable, int> res7(nmo);
    EXPECT_TRUE(res7.IsOk());
    EXPECT_NO_THROW((void)res7.Unwrap());

    Nebula::Result<int, NonMoveable> res8(nmo);
    EXPECT_TRUE(res8.IsErr());
    EXPECT_NO_THROW((void)res8.UnwrapErr());

    Nebula::Result<NonMoveable, NonMoveable> res9(nmo);
    EXPECT_TRUE(res9.IsOk());
    EXPECT_NO_THROW((void)res9.Unwrap());

    Nebula::Result<NonMoveable, NonCopyable> res10(nmo);
    EXPECT_TRUE(res10.IsOk());
    EXPECT_NO_THROW((void)res10.Unwrap());
}

TEST(ResultTest, TestMap)
{
    Nebula::Result<int, double> res(5);
    auto res2 = res.MapMove([](int val) { return val * 2; });
    EXPECT_TRUE(res2.IsOk());
    EXPECT_EQ(res2.Unwrap(), 10);

    Nebula::Result<int, double> res3(5.0);
    auto res4 = res3.MapCopy([](int val) { return val * 2; });
    EXPECT_TRUE(res4.IsErr());
    EXPECT_EQ(res4.UnwrapErr(), 5.0);

    NonMoveable nmo;
    Nebula::Result<NonMoveable, int> res5(nmo);
    auto res6 = res5.MapCopy([](NonMoveable) { return 1.0F; });
    EXPECT_TRUE(res6.IsOk());
    EXPECT_EQ(res6.Unwrap(), 1.0F);

    NonCopyable nco;
    Nebula::Result<NonCopyable, double> res7(std::move(nco));
    auto res8 = res7.MapMove([](NonCopyable) { return 1.0F; });
    EXPECT_TRUE(res8.IsOk());
    EXPECT_EQ(res8.Unwrap(), 1.0F);
}
