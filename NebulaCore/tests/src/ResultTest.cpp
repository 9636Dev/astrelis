#include "NebulaCore/Util/Result.hpp"
#include <gtest/gtest.h>

TEST(ResultTest, TestResult)
{
    Nebula::Result<int, double> res(5);
}
