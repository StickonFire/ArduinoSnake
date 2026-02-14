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
    protected: 
    void moveHeadTest(Coordinate startingHead, Coordinate expectedHead, Coordinate tail, ConstrainedVector<Direction,ledNums> startingVector, Direction input){
        Snake test(startingHead,tail,startingVector);
        startingVector.push_back(input);
        Snake expected(expectedHead,tail,startingVector);
        EXPECT_TRUE(test.moveHead(input));
        EXPECT_EQ(test,expected);
    }
};

TEST_F(SnakeMoveHeadTest,GoLeft){
    Coordinate startingHead(1,1);
    Coordinate tail(1,1);
    Coordinate expectedHead(1,0);
    Direction direction = Left;
    ConstrainedVector<Direction,ledNums> startingVector{Left};

    moveHeadTest(startingHead,expectedHead,tail,startingVector,direction);
}

TEST_F(SnakeMoveHeadTest,GoUp){
    Coordinate startingHead(4,5);
    Coordinate tail(4,5);
    Coordinate expectedHead(3,5);
    Direction direction = Up;
    ConstrainedVector<Direction,ledNums> startingVector{Left};

    moveHeadTest(startingHead,expectedHead,tail,startingVector,direction);
}

TEST_F(SnakeMoveHeadTest,GoRight){
    Coordinate startingHead(2,3);
    Coordinate tail(2,3);
    Coordinate expectedHead(2,4);
    Direction direction = Right;
    ConstrainedVector<Direction,ledNums> startingVector{Right};

    moveHeadTest(startingHead,expectedHead,tail,startingVector,direction);
}

TEST_F(SnakeMoveHeadTest,GoDown){
    Coordinate startingHead(6,7);
    Coordinate tail(6,7);
    Coordinate expectedHead(7,7);
    Direction direction = Down;
    ConstrainedVector<Direction,ledNums> startingVector{Right};

    moveHeadTest(startingHead,expectedHead,tail,startingVector,direction);
}

TEST_F(SnakeMoveHeadTest,Size3Node){
    Coordinate startingHead(2,1);
    Coordinate tail(3,3);
    Coordinate expectedHead(2,0);
    Direction direction = Left;
    ConstrainedVector<Direction,ledNums> startingVector{Left,Down,Left};

    moveHeadTest(startingHead,expectedHead,tail,startingVector,direction);
}

TEST_F(SnakeMoveHeadTest,FullNodes){
    Coordinate startingHead(1,1);
    Coordinate tail(1,1);
    Direction direction = Right;
    ConstrainedVector<Direction,ledNums> startingVector(ledNums,Right);

    Snake test(startingHead,tail,startingVector);
    EXPECT_FALSE(test.moveHead(direction));
}

class SnakeMoveTailTest : public testing::Test {
    protected:
    void testTail(Coordinate startingHead, Coordinate tail, Coordinate expectedTail, ConstrainedVector<Direction,ledNums> startingVector){
        Snake test(startingHead,tail,startingVector);
        startingVector.pop_front();
        Snake check(startingHead,expectedTail,startingVector);
        EXPECT_TRUE(test.moveTail());
        EXPECT_EQ(test,check);
    }
};

TEST_F(SnakeMoveTailTest,LastLeft){
    Coordinate startingHead(1,1);
    Coordinate tail(1,1);
    Coordinate expectedTail(1,0);
    ConstrainedVector<Direction,ledNums> startingVector(ledNums,Left);

    testTail(startingHead,tail,expectedTail,startingVector);
}

TEST_F(SnakeMoveTailTest,LastUp){
    Coordinate startingHead(1,1);
    Coordinate tail(1,1);
    Coordinate expectedTail(0,1);
    ConstrainedVector<Direction,ledNums> startingVector(ledNums,Up);

    testTail(startingHead,tail,expectedTail,startingVector);
}

TEST_F(SnakeMoveTailTest,LastRight){
    Coordinate startingHead(1,1);
    Coordinate tail(1,1);
    Coordinate expectedTail(1,2);
    ConstrainedVector<Direction,ledNums> startingVector(ledNums,Right);

    testTail(startingHead,tail,expectedTail,startingVector);
}

TEST_F(SnakeMoveTailTest,LastDown){
    Coordinate startingHead(1,1);
    Coordinate tail(1,1);
    Coordinate expectedTail(2,1);
    ConstrainedVector<Direction,ledNums> startingVector(ledNums,Down);

    testTail(startingHead,tail,expectedTail,startingVector);
}

//Tests both two nodes and an offset vector.
TEST_F(SnakeMoveTailTest,TwoNode){
    Coordinate startingHead(1,1);
    Coordinate tail(1,1);
    Coordinate expectedTail(2,1);
    std::array<Direction,ledNums> innerArray;
    innerArray[10] = Right;
    innerArray[11] = Down;
    ConstrainedVector<Direction,ledNums> startingVector(2,innerArray,10,12);

    testTail(startingHead,tail,expectedTail,startingVector);
}

TEST_F(SnakeMoveTailTest,OneNode){
    Coordinate startingHead(1,1);
    Coordinate tail(1,1);
    std::array<Direction,ledNums> innerArray;
    innerArray[10] = Down;
    ConstrainedVector<Direction,ledNums> startingVector(1,innerArray,10,11);

    Snake test(startingHead,tail,startingVector);
    EXPECT_FALSE(test.moveTail());
}

TEST_F(SnakeMoveTailTest,EmptyVector){
    Coordinate position(1,1);
    ConstrainedVector<Direction,ledNums> startingVector(0,Left);
    Snake test(position,position,startingVector);
    EXPECT_FALSE(test.moveTail());
}