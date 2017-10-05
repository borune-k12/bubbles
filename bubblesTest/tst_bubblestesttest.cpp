#include <gmock/gmock.h>

TEST(CalculatorTest, Zero) {
    EXPECT_EQ(1, (0));
}

int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}


