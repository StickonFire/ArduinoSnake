#include <array>
#include <string>
#include <gtest/gtest.h>

#include "modelControllerCommUnits.hpp"
#include "snakeModel.hpp"

using testing::Test;

const std::array<Command,3> commandList{LeftTurn,RightTurn,Pause};
const std::array<Direction,4> directionList{Up,Right,Down,Left};

std::string snakeModelToString(SnakeModel& toDescribe){
    std::string result("SnakeModel: {");
    ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> map = toDescribe.getMap();
    std::vector<std::string> mapRepresentation(map.size(),std::string(map[0].size(),' '));
    for(int width = 0; width < map.size(); width++){
        for(int height = 0; height < map[0].size(); height++){
            switch(map[width][height]){
                case Empty:
                    mapRepresentation[width][height] = ' ';
                    break;
                case SnakeTile:
                    mapRepresentation[width][height] = 'S';
                    break;
                default:
                    mapRepresentation[width][height] = 'N';
            }
        }
    }
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
    result += std::string("}\n");
    result += "Snake Front: [" + std::to_string(player.headPosition.x) + "," + std::to_string(player.headPosition.y) + "]\n";
    result += "Snake Back: [" + std::to_string(player.tailPosition.x) + "," + std::to_string(player.tailPosition.y) + "]\n";
    result += "Node Directions: [";
    for(int i = 0; i < player.nodeDirection.size(); i++){
        switch(player.nodeDirection[i]){
            case Left:
                result += "L";
                break;
            case Up:
                result += "U";
                break;
            case Right:
                result += "R";
                break;
            case Down:
                result += "D";
                break;
            default:
                result += "N";
        }
    }
    result += "]\n";
    result += "Paused: ";
    if(toDescribe.isPaused())
        result += "True\n";
    else 
        result += "False\n";
    return result + "}";
}

TEST(SnakeModelTest,SnakeModelConstructorTest){
    int mapWidth = 8;
    int mapHeight = 8;
    Coordinate playerStartingPosition(3,4);
    Direction startingDirection = Right;
    bool expectedPause = false;
    SnakeModel test(mapWidth, mapHeight, playerStartingPosition, startingDirection);

    Snake expectedSnake(startingDirection,playerStartingPosition);
    ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> expectedMap{
        ConstrainedVector<MapTileState,ledCols>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        ConstrainedVector<MapTileState,ledCols>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        ConstrainedVector<MapTileState,ledCols>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        ConstrainedVector<MapTileState,ledCols>{Empty,Empty,Empty,Empty,SnakeTile,Empty,Empty,Empty},
        ConstrainedVector<MapTileState,ledCols>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        ConstrainedVector<MapTileState,ledCols>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        ConstrainedVector<MapTileState,ledCols>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        ConstrainedVector<MapTileState,ledCols>{Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
    };
    EXPECT_EQ(test.getPlayer(),expectedSnake);
    EXPECT_EQ(test.getMap(),expectedMap);
    EXPECT_EQ(test.isPaused(),expectedPause);

    SnakeModel testFullConstructor(expectedMap,playerStartingPosition,startingDirection,expectedPause);
    EXPECT_EQ(test,testFullConstructor) << "Received: " << snakeModelToString(test) << "\nExpected: " << snakeModelToString(testFullConstructor);
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

class SnakeModelAdvanceStateTest : public testing::Test {

    protected:
    SnakeModel testModel;

    SnakeModelAdvanceStateTest(): testModel(1,1,Coordinate(0,0),Up) { }

    void checkKilled(){

    }

    void testSnakeModelEquality(SnakeModel& expectedModel,std::string message){
        EXPECT_EQ(expectedModel,testModel) << message << "\nExpected: " << snakeModelToString(expectedModel) << "\nReceived: " << snakeModelToString(testModel);
    }

    void testSnakeModelEquality(SnakeModel& expectedModel){
        testSnakeModelEquality(expectedModel,"");
    }

    void singleTileTest(Direction startDirection){
        for(Command input: commandList){
            testModel = SnakeModel(1,1,Coordinate(0,0),startDirection);
            testModel.advanceState(input);
            SnakeModel expectedModel(1,1,Coordinate(0,0),startDirection);
            if(input == Pause){
                expectedModel.togglePause();
                EXPECT_TRUE(testModel.isPaused());
            } else {
                expectedModel.setKilled(true);
                EXPECT_TRUE(testModel.isKilled());
            }
            testSnakeModelEquality(expectedModel);
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

TEST_F(SnakeModelAdvanceStateTest,PauseTest){
    testModel = SnakeModel(1,3,Coordinate(0,0),Right);
    SnakeModel expectedModel(testModel);
    testSnakeModelEquality(expectedModel,"Initialization Test");

    for(int i = 1; i < 3; i++){
        std::string messageStart = "Test position" + std::to_string(i-1) + ", case ";

        //Pause Test
        testModel.advanceState(Pause);
        expectedModel.togglePause();
        for(Command input: commandList) {
            if(input != Pause) {
                testModel.advanceState(input);
                std::string message = messageStart + "Command: " + to_string(input);
                testSnakeModelEquality(expectedModel,message);
            }
        }

        //Unpauses
        testModel.advanceState(Pause);
        expectedModel.togglePause();
        testSnakeModelEquality(expectedModel,messageStart + "Command: Pause");

        testModel.advanceState(Straight);
        ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> nextMap{ConstrainedVector<MapTileState,ledCols>(3,Empty)};
        nextMap[0][i] = SnakeTile;
        
        std::array<Direction,ledNums> expectedInner;
        expectedInner[i] = Right;
        ConstrainedVector<Direction,ledNums> expectedVector(1,expectedInner,i,(i+1));
        Snake expectedSnake(Coordinate(0,i),Coordinate(0,i),expectedVector);
        expectedModel = SnakeModel(nextMap,expectedSnake,false,false);
        testSnakeModelEquality(expectedModel,messageStart + "Moving to Next State");
    }
}

TEST_F(SnakeModelAdvanceStateTest,MovementTest){
    for(Direction testDirection: directionList){
        movementTest(testDirection);
    }
}

class SnakeModelMultinodeAdvanceStateTest : public SnakeModelAdvanceStateTest {

    protected:
    struct DoubleNodeMapDescription{
        Coordinate head;
        Coordinate tail;
        ConstrainedVector<Direction,ledNums> directions;
    };

    void multiNodeTest(DoubleNodeMapDescription start, ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> startingMap, 
            DoubleNodeMapDescription end, ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> endingMap, Command command, std::string message){

        Snake startingSnake(start.head,start.tail,start.directions);
        Snake endingSnake(end.head,end.tail,end.directions);

        testModel = SnakeModel(startingMap,startingSnake,false,false);
        SnakeModel expectedModel(endingMap,endingSnake,false,false);

        testModel.advanceState(command);
        testSnakeModelEquality(expectedModel,message);
    }

    void doubleNodeTest(DoubleNodeMapDescription start, DoubleNodeMapDescription end, Command command, std::string headDirection, std::string tailDirection){
        ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> startingMap(3,ConstrainedVector<MapTileState,ledCols>(3,Empty));
        startingMap[start.head.x][start.head.y] = SnakeTile;
        startingMap[start.tail.x][start.tail.y] = SnakeTile;
        ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> endingMap(3,ConstrainedVector<MapTileState,ledCols>(3,Empty));
        endingMap[end.head.x][end.head.y] = SnakeTile;
        endingMap[end.tail.x][end.tail.y] = SnakeTile;

        std::string message = std::string("Double Node Test failed, head turned ") + headDirection + std::string(". Tail turned ") + tailDirection;
        multiNodeTest(start,startingMap,end,endingMap,command,message);
    }
};

TEST_F(SnakeModelMultinodeAdvanceStateTest,DoubleNodeTailLeftHeadLeft){
    Coordinate startHeadPosition(1,1);
    Coordinate startTailPosition(1,0);
    Coordinate endHeadPosition(0,1);
    Coordinate endTailPosition(1,1);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Down,Right};
    ConstrainedVector<Direction,ledNums> endNodeDirection({Right,Up},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    doubleNodeTest(start,end,LeftTurn,"Left","Left");
}

TEST_F(SnakeModelMultinodeAdvanceStateTest,DoubleNodeTailStraightHeadLeft){
    Coordinate startHeadPosition(0,1);
    Coordinate startTailPosition(1,1);
    Coordinate endHeadPosition(0,0);
    Coordinate endTailPosition(0,1);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Up,Up};
    ConstrainedVector<Direction,ledNums> endNodeDirection({Up,Left},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    doubleNodeTest(start,end,LeftTurn,"Left","Straight");
}

TEST_F(SnakeModelMultinodeAdvanceStateTest,DoubleNodeTailRightHeadLeft){
    Coordinate startHeadPosition(2,1);
    Coordinate startTailPosition(2,0);
    Coordinate endHeadPosition(1,1);
    Coordinate endTailPosition(2,1);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Up,Right};
    ConstrainedVector<Direction,ledNums> endNodeDirection({Right,Up},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    doubleNodeTest(start,end,LeftTurn,"Left","Right");
}

TEST_F(SnakeModelMultinodeAdvanceStateTest,DoubleNodeTailLeftHeadStraight){
    Coordinate startHeadPosition(1,1);
    Coordinate startTailPosition(2,1);
    Coordinate endHeadPosition(0,1);
    Coordinate endTailPosition(1,1);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Right,Up};
    ConstrainedVector<Direction,ledNums> endNodeDirection({Up,Up},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    doubleNodeTest(start,end,Straight,"Straight","Left");
}

TEST_F(SnakeModelMultinodeAdvanceStateTest,DoubleNodeTailStraightHeadStraight){
    Coordinate startHeadPosition(1,1);
    Coordinate startTailPosition(1,2);
    Coordinate endHeadPosition(1,0);
    Coordinate endTailPosition(1,1);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Left,Left};
    ConstrainedVector<Direction,ledNums> endNodeDirection({Left,Left},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    doubleNodeTest(start,end,Straight,"Straight","Straight");
}

TEST_F(SnakeModelMultinodeAdvanceStateTest,DoubleNodeTailRightHeadStraight){
    Coordinate startHeadPosition(1,2);
    Coordinate startTailPosition(0,2);
    Coordinate endHeadPosition(2,2);
    Coordinate endTailPosition(1,2);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Right,Down};
    ConstrainedVector<Direction,ledNums> endNodeDirection({Down,Down},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    doubleNodeTest(start,end,Straight,"Straight","Right");
}

TEST_F(SnakeModelMultinodeAdvanceStateTest,DoubleNodeTailLeftHeadRight){
    Coordinate startHeadPosition(1,1);
    Coordinate startTailPosition(1,2);
    Coordinate endHeadPosition(0,1);
    Coordinate endTailPosition(1,1);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Up,Left};
    ConstrainedVector<Direction,ledNums> endNodeDirection({Left,Up},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    doubleNodeTest(start,end,RightTurn,"Right","Left");
}

TEST_F(SnakeModelMultinodeAdvanceStateTest,DoubleNodeTailStraightHeadRight){
    Coordinate startHeadPosition(2,2);
    Coordinate startTailPosition(1,2);
    Coordinate endHeadPosition(2,1);
    Coordinate endTailPosition(2,2);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Down,Down};
    ConstrainedVector<Direction,ledNums> endNodeDirection({Down,Left},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    doubleNodeTest(start,end,RightTurn,"Right","Straight");
}

TEST_F(SnakeModelMultinodeAdvanceStateTest,DoubleNodeTailRightHeadRight){
    Coordinate startHeadPosition(2,0);
    Coordinate startTailPosition(2,1);
    Coordinate endHeadPosition(1,0);
    Coordinate endTailPosition(2,0);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Down,Left};
    ConstrainedVector<Direction,ledNums> endNodeDirection({Left,Up},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    doubleNodeTest(start,end,RightTurn,"Right","Right");
}

TEST_F(SnakeModelMultinodeAdvanceStateTest,SnakeLoops){
    ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> map{
        {SnakeTile,SnakeTile,SnakeTile},
        {SnakeTile,Empty,SnakeTile},
        {SnakeTile,SnakeTile,SnakeTile}
    };
    Coordinate startHeadPosition(1,2);
    Coordinate startTailPosition(0,2);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Up,Left,Left,Down,Down,Right,Right,Up};
    Coordinate endHeadPosition(0,2);
    Coordinate endTailPosition(0,1);
    ConstrainedVector<Direction,ledNums> endNodeDirection({Left,Left,Down,Down,Right,Right,Up,Up},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    multiNodeTest(start,map,end,map,Straight,"Snake Donut Loop Failed");
}

TEST_F(SnakeModelMultinodeAdvanceStateTest,SnakeFullScreen){
    ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> map{
        {SnakeTile,SnakeTile},
        {SnakeTile,SnakeTile}
    };
    Coordinate startHeadPosition(0,1);
    Coordinate startTailPosition(0,0);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Left,Down,Right,Up};
    Coordinate endHeadPosition(0,0);
    Coordinate endTailPosition(1,0);
    ConstrainedVector<Direction,ledNums> endNodeDirection({Down,Right,Up,Left},1);

    DoubleNodeMapDescription start{startHeadPosition,startTailPosition,startNodeDirection};
    DoubleNodeMapDescription end{endHeadPosition,endTailPosition,endNodeDirection};
    multiNodeTest(start,map,end,map,LeftTurn,"Snake Full Screen Loop Failed");
}