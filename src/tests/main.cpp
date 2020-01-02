#include <common/logger.h>
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int res = RUN_ALL_TESTS();
    ASSERT_MSG(0 == res, "Tests failed!");
    return res;
}
