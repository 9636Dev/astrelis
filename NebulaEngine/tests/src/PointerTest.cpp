#include <gtest/gtest.h>

#include "NebulaEngine/Core/Pointer.hpp"

using Nebula::RefPtr, Nebula::ScopedPtr, Nebula::OwnedPtr, Nebula::RawRef;

TEST(PointerTest, PointerCreation) {
    ScopedPtr<int> scopedPtr1 = nullptr;
    EXPECT_EQ(scopedPtr1, nullptr);

    RefPtr<int> refPtr1 = nullptr;
    EXPECT_EQ(refPtr1, nullptr);

    OwnedPtr<int*> ownedPtr1 = nullptr;
    EXPECT_EQ(ownedPtr1, nullptr);

    RawRef<int*> rawPtr1 = nullptr;
    EXPECT_EQ(rawPtr1, nullptr);
}
