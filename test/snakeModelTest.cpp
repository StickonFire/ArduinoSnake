#include <array>
#include <string>
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

    std::string snakeModelToString(SnakeModel& toDescribe){
        std::string result("SnakeModel: {");
        std::vector<std::vector<MapTileState>> map = toDescribe.getMap();
        std::vector<std::string> mapRepresentation(map.size(),std::string(map[0].size(),' '));
        Snake player(toDescribe.getPlayer());
        Coordinate headPosition(player.headPosition);
        char snakeDesign;
        switch(player.currentDirection){
            case Up:
                snakeDesign = 'v';
                break;
            case Right:
                snakeDesign = '<';
                break;
            case Down:
                snakeDesign = 'n';
                break;
            case Left:
                snakeDesign = '>';
        }
        mapRepresentation[headPosition.x][headPosition.y] = snakeDesign;
        result += "Map: {\n";
        for(std::string row: mapRepresentation)
            result += std::string("[") + row + "]\n";
        result += std::string("}");
        result += "Paused: " + toDescribe.isPaused() ? "True" : "False" + std::string("\n");
        return result + "}";
    }

    void checkKilled(){

    }

    void testSnakeModelEquality(SnakeModel& expectedModel){
        EXPECT_EQ(expectedModel,testModel) << "Expected: " << snakeModelToString(expectedModel) << "\nReceived: " << snakeModelToString(testModel);
    }

    void singleTileTest(Direction startDirection){
        for(Command input: commandList){
            testModel = SnakeModel(1,1,Coordinate(0,0),startDirection);
            testModel.advanceState(input);
            if(input == Pause){
                SnakeModel expectedModel(1,1,Coordinate(0,0),startDirection);
                expectedModel.togglePause();
                testSnakeModelEquality(expectedModel);
                EXPECT_TRUE(testModel.isPaused());
            } else {
                checkKilled();
            }
        }
    }

    void movementTest(Direction startDirection){
        for(Command input: commandList){
            testModel = SnakeModel(3,3,Coordinate(1,1),startDirection);
            testModel.advanceState(input);
            if(input == Pause){
                SnakeModel expectedModel(3,3,Coordinate(1,1),startDirection);
                expectedModel.togglePause();
                EXPECT_EQ(expectedModel,testModel);
                EXPECT_TRUE(testModel.isPaused());
            } else {
                SnakeModel expectedModel(3,3,Coordinate(1,1),startDirection);
                switch(changeDirection(startDirection,input)){
                    case Up:
                        expectedModel = SnakeModel(3,3,Coordinate(0,1),Up);
                        break;
                    case Right:
                        expectedModel = SnakeModel(3,3,Coordinate(1,2),Right);
                        break;
                    case Down:
                        expectedModel = SnakeModel(3,3,Coordinate(2,1),Down);
                        break;
                    case Left:
                        expectedModel = SnakeModel(3,3,Coordinate(1,0),Left);
                }
                testSnakeModelEquality(testModel);
            }
        }
    }
};

TEST_F(SnakeModelAdvanceStateTest,SingleTileTest){
    for(Direction testDirection: directionList){
        singleTileTest(testDirection);
    }
}
TEST_F(SnakeModelAdvanceStateTest,MovementTest){
    for(Direction testDirection: directionList){
        movementTest(testDirection);
    }
}

}