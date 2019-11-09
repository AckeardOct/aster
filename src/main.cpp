#include "common/game_window.h"

#include <gtest/gtest.h>

TEST(SimpleTest, Primitive)
{
    EXPECT_EQ(true, true);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    if (ret) {
        return ret;
    }

    GameWindow window(argc, argv);
    window.runLoop();

    return 0;
}
