#include <gtest/gtest.h>

#include "NebulaCore/Log/Log.hpp"
#include "NebulaCore/Util/Pointer.hpp"

// NOLINTNEXTLINE
std::size_t num = 0;

struct TestStruct // NOLINT
{
    ~TestStruct() { num++; }
};

TEST(PointerTest, PtrTest)
{
    gsl::owner<int*> ptr = new int(5);
    Nebula::Ptr<int> ptr1(ptr);
    EXPECT_EQ(*ptr1, 5);

    num = 0;
    {
        Nebula::Ptr<TestStruct> ptr2(new TestStruct());
    }
    EXPECT_EQ(num, 1);
}

TEST(PointerTest, RefTest)
{
    Nebula::Log::Init();
    Nebula::Ptr<int> value(new int(5));
    auto ref = value.GetRef();
    EXPECT_EQ(*ref, 5);

    num = 0;
    {
        Nebula::Ptr<TestStruct> ptr(new TestStruct());
        {
            auto ref2 = ptr.GetRef();
        }
        EXPECT_EQ(num, 0);
    }
    EXPECT_EQ(num, 1);
}

TEST(PointerTest, RefFromValueTest)
{
    int value = 5;
    auto ref  = Nebula::Ref<int>(value);

    EXPECT_EQ(*ref, 5);

    num = 0;
    {
        TestStruct value;
        {
            auto ref2 = Nebula::Ref<TestStruct>(value);
        }
        EXPECT_EQ(num, 0);

        // Destructor is called by the end of the scope
    }
    EXPECT_EQ(num, 1);
}
