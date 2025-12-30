#include <array>
#include <gtest/gtest.h>

#include "snakeModel.hpp"

using testing::Test;

TEST(SnakeModelTest,SnakeConstructorTest){
    Direction expectedDirection = Left;
    Coordinate expectedPosition(1,2);

    Snake test(expectedDirection,expectedPosition);

    EXPECT_EQ(test.currentDirection,expectedDirection);
    EXPECT_EQ(test.headPosition,expectedPosition);
}

TEST(SnakeModelTest,SnakeModelConstructorTest){
    int mapWidth = 8;
    int mapHeight = 8;
    Coordinate playerStartingPosition(3,4);
    Direction startingDirection = Right;
    bool expectedPause = false;
    SnakeModel test(mapWidth, mapHeight, playerStartingPosition, startingDirection);

    Snake expectedSnake(startingDirection,playerStartingPosition);
    std::vector<std::vector<MapTileState>> expectedMap{
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,SnakeTile,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        std::vector<MapTileState>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
    };
    EXPECT_EQ(test.getPlayer(),expectedSnake);
    EXPECT_EQ(test.getMap(),expectedMap);
    EXPECT_EQ(test.isPaused(),expectedPause);

    SnakeModel testFullConstructor(expectedMap,playerStartingPosition,startingDirection,expectedPause);
    EXPECT_EQ(test,testFullConstructor);
}

TEST(SnakeModelTest,ChangeDirectionTest){
    EXPECT_EQ(changeDirection(Left,LeftTurn),Down);
    EXPECT_EQ(changeDirection(Left,Straight),Left);
    EXPECT_EQ(changeDirection(Left,RightTurn),Up);
    EXPECT_EQ(changeDirection(Up,LeftTurn),Left);
    EXPECT_EQ(changeDirection(Up,Straight),Up);
    EXPECT_EQ(changeDirection(Up,RightTurn),Right);
    EXPECT_EQ(changeDirection(Right,LeftTurn),Up);
    EXPECT_EQ(changeDirection(Right,Straight),Right);
    EXPECT_EQ(changeDirection(Right,RightTurn),Down);
    EXPECT_EQ(changeDirection(Down,LeftTurn),Right);
    EXPECT_EQ(changeDirection(Down,Straight),Down);
    EXPECT_EQ(changeDirection(Down,RightTurn),Left);
}

const std::array<Command,3> commandList{LeftTurn,RightTurn,Pause};
const std::array<Direction,4> directionList{Up,Right,Down,Left};

class SnakeModelAdvanceStateTest : public testing::Test {

    protected:
    SnakeModel testModel;

    SnakeModelAdvanceStateTest(): testModel(1,1,Coordinate(0,0),Up) { }

    void checkKilled(){

    }

    void singleTileTest(Direction startDirection){
        for(Command input: commandList){
            testModel = SnakeModel(1,1,Coordinate(0,0),startDirection);
            testModel.advanceState(input);
            if(input == Pause){
                SnakeModel expectedModel(1,1,Coordinate(0,0),startDirection);
                expectedModel.togglePause();
                EXPECT_EQ(expectedModel,testModel);
                EXPECT_TRUE(testModel.isPaused());
            } else {
                checkKilled();
            }
        }
    }

};

TEST_F(SnakeModelAdvanceStateTest,SingleTileTest){
    for(Direction testDirection: directionList){
        singleTileTest(testDirection);
    }
}
}