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
    Coordinate expectedHeadPosition(1,2);
    Coordinate expectedTailPosition(1,2);
    ConstrainedVector<Direction,ledNums> expectedDirections(1,Left);

    Snake test(expectedHeadPosition,expectedTailPosition,expectedDirections);

    EXPECT_EQ(test.headPosition,expectedHeadPosition);
    EXPECT_EQ(test.tailPosition,expectedTailPosition);
    EXPECT_EQ(test.nodeDirection,expectedDirections);
}

TEST(SnakeEqualityTest,Equals){
    Direction expectedDirection = Left;
    Coordinate expectedHeadPosition(1,2);
    Coordinate expectedTailPosition(1,2);
    ConstrainedVector<Direction,ledNums> expectedDirections(1,Left);

    Snake test(expectedHeadPosition,expectedTailPosition,expectedDirections);
    Snake shouldEqual(expectedHeadPosition,expectedTailPosition,expectedDirections);
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

    Snake test(expectedHeadPosition,expectedTailPosition,expectedDirections);
    Snake check(expectedHeadPosition,expectedTailPosition,expectedDirections);
    EXPECT_EQ(test,check);
    check = Snake(wrongHead,expectedTailPosition,expectedDirections);
    EXPECT_FALSE(test == check) << "Unequal head but equality returned true.";
    check = Snake(expectedHeadPosition,wrongTail,expectedDirections);
    EXPECT_FALSE(test == check) << "Unequal tail but equality returned true.";
    check = Snake(expectedHeadPosition,expectedTailPosition,wrongDirections);
    EXPECT_FALSE(test == check) << "Unequal vector of directions but equality returned true.";
}

class SnakeMoveHeadTest : public testing::Test {
    //Coordinate startingHead;
    //Coordinate expectedHead;

};

TEST_F(SnakeMoveHeadTest,GoLeft){
    Coordinate expectedHead(1,1);
    Coordinate startingHead(2,2);
    Coordinate tail(1,1);
    Direction direction = Left;
    ConstrainedVector<Direction,ledNums> startingVector{Left};


    Snake test(startingHead,tail,startingVector);
    startingVector.push_back(direction);
    Snake expected(expectedHead,tail,startingVector);
    EXPECT_TRUE(test.moveHead(direction));
    EXPECT_EQ(test,expected);
}

