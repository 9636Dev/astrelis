#include <gtest/gtest.h>

#include "NebulaEngine/Core/Pointer.hpp"

using Nebula::RefPtr, Nebula::ScopedPtr, Nebula::OwnedPtr, Nebula::RawRef;

TEST(PointerTest, PointerAssignmentCreation)
{
    ScopedPtr<int> scopedPtr1 = nullptr;
    EXPECT_EQ(scopedPtr1, nullptr);

    RefPtr<int> refPtr1 = nullptr;
    EXPECT_EQ(refPtr1, nullptr);

    OwnedPtr<int*> ownedPtr1 = nullptr;
    EXPECT_EQ(ownedPtr1, nullptr);

    RawRef<int*> rawPtr1 = nullptr;
    EXPECT_EQ(rawPtr1, nullptr);
}

TEST(PointerTest, PointerCreation)
{
    ScopedPtr<int> scopedPtr1 = ScopedPtr<int>::Create(5);
    EXPECT_EQ(*scopedPtr1, 5);

    RefPtr<int> refPtr1 = RefPtr<int>::Create(5);
    EXPECT_EQ(*refPtr1, 5);

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    OwnedPtr<int*> ownedPtr1 = OwnedPtr<int*>::Create(5);
    EXPECT_EQ(*ownedPtr1, 5);

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    RawRef<int*> rawPtr1 = static_cast<RawRef<int*>>(new int(5));
    EXPECT_EQ(*rawPtr1, 5);
}

TEST(PointerTest, WrapRawPointer)
{
    ScopedPtr<int> scopedPtr1 = ScopedPtr<int>(new int(5));
    EXPECT_EQ(*scopedPtr1, 5);
    scopedPtr1.Reset();
    EXPECT_EQ(scopedPtr1, nullptr);

    RefPtr<int> refPtr1 = RefPtr<int>(new int(5));
    EXPECT_EQ(*refPtr1, 5);
    refPtr1.Reset();
    EXPECT_EQ(refPtr1, nullptr);

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    OwnedPtr<int*> ownedPtr1 = OwnedPtr<int*>(new int(5));
    EXPECT_EQ(*ownedPtr1, 5);
    ownedPtr1.Reset();
    EXPECT_EQ(ownedPtr1, nullptr);

    // No need to test RawPtr
}

TEST(PointerTest, PointerCopy)
{
    // Scoped ptrs cannot be copied

    RefPtr<int> refPtr1 = RefPtr<int>::Create(5);
    RefPtr<int> refPtr2 = refPtr1;

    (void)refPtr2;
    EXPECT_EQ(*refPtr1, 5);
    EXPECT_EQ(*refPtr2, 5);

    // Owned ptrs cannot be copied

    // No need to test RawPtr
}

TEST(PointerTest, PointerMove)
{
    ScopedPtr<int> scopedPtr1 = ScopedPtr<int>::Create(5);
    ScopedPtr<int> scopedPtr2 = std::move(scopedPtr1);

    (void)scopedPtr1;
    EXPECT_EQ(*scopedPtr2, 5);

    RefPtr<int> refPtr1 = RefPtr<int>::Create(5);
    RefPtr<int> refPtr2 = std::move(refPtr1);

    (void)refPtr1;
    EXPECT_EQ(*refPtr2, 5);

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    OwnedPtr<int*> ownedPtr1 = OwnedPtr<int*>::Create(5);
    OwnedPtr<int*> ownedPtr2 = std::move(ownedPtr1);
    EXPECT_EQ(*ownedPtr2, 5);

    // No need to test RawPtr
}
