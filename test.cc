#include <gtest/gtest.h>

// Function to be tested
int add(int a, int b) {
    return a + b;
}

// Test case
TEST(AdditionTest, PositiveNumbers) {
    EXPECT_EQ(add(1, 2), 3);
    EXPECT_EQ(add(5, 5), 10);
}

TEST(AdditionTest, NegativeNumbers) {
    EXPECT_EQ(add(-1, -1), -2);
    EXPECT_EQ(add(-5, -3), -8);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

