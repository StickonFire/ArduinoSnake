#include <array>
#include <string>
#include <map>
#include <cstdint>
#include <gtest/gtest.h>

#include "modelControllerCommUnits.hpp"
#include "snakeModel.hpp"
#include "controller.hpp"

using testing::Test;

TEST(ControllerTests,ControlsToSoftwareConverterTest){
    std::map<std::array<bool,buttonNumbers>,Command> expected{
        {std::array<bool,buttonNumbers>{false,false,false},Straight},
        {std::array<bool,buttonNumbers>{false,false,true},RightTurn},
        {std::array<bool,buttonNumbers>{false,true,false},Pause},
        {std::array<bool,buttonNumbers>{false,true,true},Pause},
        {std::array<bool,buttonNumbers>{true,false,false},LeftTurn},
        {std::array<bool,buttonNumbers>{true,false,true},Straight},
        {std::array<bool,buttonNumbers>{true,true,false},Pause},
        {std::array<bool,buttonNumbers>{true,true,true},Pause},
    };
    ControlsParser test;
    for(std::pair<std::array<bool,buttonNumbers>,Command> testFields: expected){
        EXPECT_EQ(test.parseButtons(testFields.first),testFields.second);
    }
}

class ConstrainedVectorToArrayHelperFunctions {
    public:
        static void setUpExpected(int startRow, int startCol, ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> init, uint8_t toInit[ledRows][ledCols]){
            for(int i = 0; i < ledRows;i++){
                for(int j = 0; j < ledCols; j++){
                    toInit[i][j] = 1;
                }
            }
            for(int i = 0; i < init.size(); i++){
                for(int j = 0; j < init[0].size(); j++){
                    toInit[i+startRow][j+startCol] = init[i][j];
                }
            }
        }

        static void checkOutputEquality(uint8_t expected[ledRows][ledCols], uint8_t result[ledRows][ledCols], std::string message){
            bool equal = true;
            std::string expectedString = "TOP\n";
            std::string resultString = "TOP\n";
            for(int i = 0; i < ledRows; i++){
                for(int j = 0; j < ledCols; j++){
                    expectedString += std::to_string(int(expected[i][j])) + ",";
                    resultString += std::to_string(int(result[i][j])) + ",";
                    equal = (result[i][j] == expected[i][j]) && equal;
                }
                expectedString += "\n";
                resultString += "\n";
            }
            EXPECT_TRUE(equal) << message << "Expected: [\n" << expectedString << "]\nRECEIVED: [\n" << resultString << "]";
        }
};

class ConstrainedVectorToArrayTest : public testing::Test {
    protected:
        ConstrainedVectorToArray test;
        ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> input;
        uint8_t result[ledRows][ledCols];
        uint8_t expected[ledRows][ledCols];

        ConstrainedVectorToArrayTest(){
            for(int i = 0; i < ledRows; i++){
                for(int j = 0; j < ledCols; j++){
                    result[i][j] = 0;
                    expected[i][j] = 1;
                }
            }
        }

        void setUpExpected(int startRow, int startCol, ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> init){
            ConstrainedVectorToArrayHelperFunctions::setUpExpected(startRow,startCol,init,expected);
        }

        void checkEquality(std::string message){
            ConstrainedVectorToArrayHelperFunctions::checkOutputEquality(expected,result,message);
        }
};

TEST_F(ConstrainedVectorToArrayTest,OddWidth){
    int windowRow = 3;
    int windowCol = 4;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty,SnakeTile,Empty},
    };
    ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> initExpected{
        {0,1,0}
    };
    setUpExpected(windowRow,windowCol,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with oddWidth failed.");
}

TEST_F(ConstrainedVectorToArrayTest,EvenWidth){
    int windowRow = 3;
    int windowCol = 5;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty,Empty},
    };
    ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> initExpected{
        {0,0}
    };
    setUpExpected(windowRow,windowCol,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with evenWidth failed.");
}

TEST_F(ConstrainedVectorToArrayTest,FullWidth){
    int windowRow = 3;
    int windowCol = 0;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        ConstrainedVector<MapTileState,ledCols>(ledCols,Empty)
    };
    ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> initExpected{
        ConstrainedVector<uint8_t,ledCols>(ledCols,0)
    };
    setUpExpected(windowRow,windowCol,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with FullWidth failed.");
}

TEST_F(ConstrainedVectorToArrayTest,OddHeight){
    int windowRow = 2;
    int windowCol = 5;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty},
        {SnakeTile},
        {Empty}
    };
    ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> initExpected{
        {0},
        {1},
        {0}
    };
    setUpExpected(windowRow,windowCol,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with OddHeight failed.");
}

TEST_F(ConstrainedVectorToArrayTest,EvenHeight){
    int windowRow = 1;
    int windowCol = 5;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty},
        {Empty},
        {SnakeTile},
        {SnakeTile},
        {Empty},
        {Empty}
    };
    ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> initExpected{
        {0},
        {0},
        {1},
        {1},
        {0},
        {0}
    };
    setUpExpected(windowRow,windowCol,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with EvenHeight failed.");
}

TEST_F(ConstrainedVectorToArrayTest,FullHeight){
    int windowRow = 0;
    int windowCol = 5;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty},
        {Empty},
        {Empty},
        {SnakeTile},
        {SnakeTile},
        {Empty},
        {Empty},
        {Empty}
    };
    ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> initExpected{
        {0},
        {0},
        {0},
        {1},
        {1},
        {0},
        {0},
        {0}
    };
    setUpExpected(windowRow,windowCol,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with FullHeight failed.");
}

TEST_F(ConstrainedVectorToArrayTest,FullScreen){
    int windowRow = 0;
    int windowCol = 0;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty},
        {Empty,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,Empty},
        {Empty,SnakeTile,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,SnakeTile,Empty},
        {Empty,SnakeTile,Empty,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,Empty,SnakeTile,Empty},
        {Empty,SnakeTile,Empty,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,Empty,SnakeTile,Empty},
        {Empty,SnakeTile,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,SnakeTile,Empty},
        {Empty,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,SnakeTile,Empty},
        {Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty,Empty}
    };
    ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> initExpected{
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,1,1,1,1,1,1,1,0},
        {0,1,0,0,0,0,0,0,0,0,1,0},
        {0,1,0,1,1,1,1,1,1,0,1,0},
        {0,1,0,1,1,1,1,1,1,0,1,0},
        {0,1,0,0,0,0,0,0,0,0,1,0},
        {0,1,1,1,1,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0}
    };
    setUpExpected(windowRow,windowCol,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with FullScreen failed.");
}

TEST_F(ConstrainedVectorToArrayTest,InsideCheck){
    int windowRow = 3;
    int windowCol = 4;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty,SnakeTile,Empty},
        {SnakeTile,Empty,Empty}
    };
    ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> initExpected{
        {0,1,0},
        {1,0,0}
    };
    setUpExpected(windowRow,windowCol,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with unflippable matrix failed.");
}

TEST(ControllerTest,MoveNoKillTest){
    ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> map{
        {SnakeTile,Empty},
        {SnakeTile,SnakeTile}
    };
    Coordinate startHeadPosition(1,1);
    Coordinate startTailPosition(0,0);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Left,Down,Right};
    Coordinate endHeadPosition(0,1);
    Coordinate endTailPosition(1,0);
    ConstrainedVector<Direction,ledNums> endNodeDirection({Down,Right,Up},1);
    Snake startingSnake(startHeadPosition,startTailPosition,startNodeDirection);
    Snake endingSnake(endHeadPosition,endTailPosition,endNodeDirection);

    SnakeModel innerModel(map,startingSnake,false,false);
    ArduinoSnakeController test(ControlsParser(),ConstrainedVectorToArray(), innerModel);

    uint8_t result[ledRows][ledCols];
    ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> expectedVector{
        {0,1},
        {1,1}
    };
    uint8_t expected[ledRows][ledCols];
    ConstrainedVectorToArrayHelperFunctions::setUpExpected(3,5,expectedVector,expected);

    std::array<bool,3> inputs = {true,false,false};
    EXPECT_EQ(test.moveStep(inputs,result),Running);
    ConstrainedVectorToArrayHelperFunctions::checkOutputEquality(expected,result,"MoveNoKill");
}

TEST(ControllerTest,MoveKillTest){
    ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> map{
        {SnakeTile,SnakeTile,SnakeTile},
        {SnakeTile,SnakeTile,Empty}
    };
    Coordinate startHeadPosition(1,1);
    Coordinate startTailPosition(0,2);
    ConstrainedVector<Direction,ledNums> startNodeDirection{Up,Left,Left,Down,Right};
    Coordinate endHeadPosition(1,1);
    Coordinate endTailPosition(0,2);
    ConstrainedVector<Direction,ledNums> endNodeDirection{Up,Left,Left,Down,Right};
    Snake startingSnake(startHeadPosition,startTailPosition,startNodeDirection);
    Snake endingSnake(endHeadPosition,endTailPosition,endNodeDirection);

    SnakeModel innerModel(map,startingSnake,false,false);
    ArduinoSnakeController test(ControlsParser(),ConstrainedVectorToArray(), innerModel);

    uint8_t result[ledRows][ledCols];
    ConstrainedVector<ConstrainedVector<uint8_t,ledCols>,ledRows> expectedVector{
        {1,1,1},
        {1,1,0}
    };
    uint8_t expected[ledRows][ledCols];
    ConstrainedVectorToArrayHelperFunctions::setUpExpected(3,4,expectedVector,expected);

    std::array<bool,3> inputs = {true,false,false};
    EXPECT_EQ(test.moveStep(inputs,result),Killed);
    ConstrainedVectorToArrayHelperFunctions::checkOutputEquality(expected,result,"MoveNoKill");
}