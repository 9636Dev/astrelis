#include <gtest/gtest.h>

#include "NebulaEngine/Core/Pointer.hpp"

using Nebula::RefPtr, Nebula::ScopedPtr, Nebula::OwnedPtr, Nebula::RawRef;

TEST(PointerTest, PointerAssignmentCreation) {
    ScopedPtr<int> scopedPtr1 = nullptr;
    EXPECT_EQ(scopedPtr1, nullptr);

    RefPtr<int> refPtr1 = nullptr;
    EXPECT_EQ(refPtr1, nullptr);

    OwnedPtr<int*> ownedPtr1 = nullptr;
    EXPECT_EQ(ownedPtr1, nullptr);

    RawRef<int*> rawPtr1 = nullptr;
    EXPECT_EQ(rawPtr1, nullptr);
}

TEST(PointerTest, PointerCreation) {
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
