#include <gtest/gtest.h>
#include "factorial.hpp" 

TEST(FactorialTest, HandlesZero) {
    EXPECT_EQ(factorial(0), 1); 
}

TEST(FactorialTest, HandlesPositiveInput) {
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(2), 2);
    EXPECT_EQ(factorial(3), 6);
    EXPECT_EQ(factorial(5), 120); 
}

TEST(FactorialTest, HandlesNegativeInput) {
    EXPECT_THROW(factorial(-1), std::invalid_argument);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
