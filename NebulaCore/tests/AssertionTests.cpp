#include <gtest/gtest.h>
#define NEBULA_ASSERT_ONFAIl throw std::runtime_error("Assertion Failed")
#include "NebulaCore/Assert.hpp"

TEST(AssertionTests, AssertTrue)
{
    NEB_ASSERT(true, "This should not fail");
}

TEST(AssertionTests, AssertFalse)
{
    EXPECT_DEATH(NEB_ASSERT(false, "This should fail"), "");
}
