#include "common/game_window.h"

#include <gtest/gtest.h>

#include "tests/test_lua.h"
#include "tests/test_rttr.h"

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

TEST(SimpleTest, Primitive)
{
    EXPECT_EQ(true, true);
}
