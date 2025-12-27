#include <gtest/gtest.h>

#include "snakeModel.hpp"

TEST(SnakeModelTest,SnakeConstructorTest){
    Direction expectedDirection = Left;
    int expectedPositionX = 1;
    int expectedPositionY = 2;

    Snake test(expectedDirection,expectedPositionX,expectedPositionY);

    EXPECT_EQ(test.currentDirection,expectedDirection);
    EXPECT_EQ(test.positionX,expectedPositionX);
    EXPECT_EQ(test.positionY,expectedPositionY);
}

