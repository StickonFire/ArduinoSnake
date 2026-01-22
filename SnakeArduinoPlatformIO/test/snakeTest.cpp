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

TEST(SnakeEqualityTest,Equals){
    Direction expectedDirection = Left;
    Coordinate expectedHeadPosition(1,2);
    Coordinate expectedTailPosition(1,2);
    ConstrainedVector<Direction,ledNums> expectedDirections(1,Left);

    Snake test(expectedDirection,expectedHeadPosition,expectedTailPosition,expectedDirections);
    Snake shouldEqual(expectedDirection,expectedHeadPosition,expectedTailPosition,expectedDirections);
    EXPECT_EQ(test,shouldEqual);
}

TEST(SnakeEqualityTest,CycleUnequal){
    Direction expectedDirection = Left;
    Direction wrongDirection = Right;
    Coordinate expectedHeadPosition(1,2);
    Coordinate wrongHead(2,2);
    Coordinate expectedTailPosition(3,2);
    Coordinate wrongTail(2,2);
    ConstrainedVector<Direction,ledNums> expectedDirections(1,Left);
    ConstrainedVector<Direction,ledNums> wrongDirections(1,Right);

    Snake test(expectedDirection,expectedHeadPosition,expectedTailPosition,expectedDirections);
    Snake check(expectedDirection,expectedHeadPosition,expectedTailPosition,expectedDirections);
    EXPECT_EQ(test,check);
    check = Snake(wrongDirection,expectedHeadPosition,expectedTailPosition,expectedDirections);
    EXPECT_FALSE(test == check) << "Unequal Direction but equality returned true.";
    check = Snake(expectedDirection,wrongHead,expectedTailPosition,expectedDirections);
    EXPECT_FALSE(test == check) << "Unequal head but equality returned true.";
    check = Snake(expectedDirection,expectedHeadPosition,wrongTail,expectedDirections);
    EXPECT_FALSE(test == check) << "Unequal tail but equality returned true.";
    check = Snake(expectedDirection,expectedHeadPosition,expectedTailPosition,wrongDirections);
    EXPECT_FALSE(test == check) << "Unequal vector of directions but equality returned true.";
}
