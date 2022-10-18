#include <gtest/gtest.h>

#include <handler.h>

TEST(exampleTests, DemonstrateGTestMacros) {
    EXPECT_TRUE(true);
    EXPECT_TRUE(2 == 2);

    std::string test;
    recieveMessage(test);
}