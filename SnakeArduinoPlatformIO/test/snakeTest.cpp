#include <array>
#include <string>
#include <gtest/gtest.h>

#include "modelControllerCommUnits.hpp"
#include "snakeModel.hpp"

using testing::Test;

TEST(SnakeConstuctorTest,SnakeHeadOnlyConstructor){
    Direction expectedDirection = Left;
    Coordinate expectedHeadPosition(1,2);
    Coordinate expectedTailPosition(1,2);
    ConstrainedVector<Direction,ledNums> expectedDirections(1,Left);

    Snake test(expectedDirection,expectedHeadPosition);

    EXPECT_EQ(test.currentDirection,expectedDirection);
    EXPECT_EQ(test.headPosition,expectedHeadPosition);
    EXPECT_EQ(test.tailPosition,expectedTailPosition);
    EXPECT_EQ(test.nodeDirection,expectedDirections);
}

TEST(SnakeConstructorTest,SnakeFullConstructor){
    Direction expectedDirection = Left;
    Coordinate expectedHeadPosition(1,2);
    Coordinate expectedTailPosition(1,2);
    ConstrainedVector<Direction,ledNums> expectedDirections(1,Left);

    Snake test(expectedDirection,expectedHeadPosition,expectedTailPosition,expectedDirections);

    EXPECT_EQ(test.currentDirection,expectedDirection);
    EXPECT_EQ(test.headPosition,expectedHeadPosition);
    EXPECT_EQ(test.tailPosition,expectedTailPosition);
    EXPECT_EQ(test.nodeDirection,expectedDirections);
}

