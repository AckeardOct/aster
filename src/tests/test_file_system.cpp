#include <gtest/gtest.h>

#include "common/file_system.h"

static const std::string TEST_DIR = "./tests/";

TEST(FileSystem, Exists)
{
    const std::string file = TEST_DIR + "/cat.txt";
    EXPECT_TRUE(fs::exists(file));

    const std::string wrongFile = TEST_DIR + "/wrong-file.txt";
    EXPECT_FALSE(fs::exists(wrongFile));
}

TEST(FileSystem, Cat)
{
    const std::string file = TEST_DIR + "/cat.txt";
    std::string text = fs::cat(file);
    const std::string_view expect = "Cat is working,\n"
                                    "correctly!\n";
    EXPECT_EQ(expect, text);
}
