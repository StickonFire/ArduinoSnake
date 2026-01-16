#include <array>
#include <string>
#include <map>
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

class ConstrainedVectorToArrayTest : public testing::Test {
    protected:
        ConstrainedVectorToArray test;
        ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> input;
        bool result[ledRows][ledCols];
        bool expected[ledRows][ledCols];

        ConstrainedVectorToArrayTest(){
            for(int i = 0; i < ledRows; i++){
                for(int j = 0; j < ledCols; j++){
                    result[i][j] = false;
                    expected[i][j] = true;
                }
            }
        }

        void setUpExpected(int startRow, int startCol, ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> init){
            for(int i = 0; i < init.size(); i++){
                for(int j = 0; j < init[0].size(); j++){
                    expected[i+startRow][j+startCol] = init[i][j];
                }
            }
        }

        void checkEquality(std::string message){
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

TEST_F(ConstrainedVectorToArrayTest,OddWidth){
    int windowRow = 3;
    int windowCol = 4;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty,SnakeTile,Empty},
    };
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        {false,true,false}
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
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        {false,false}
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
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        ConstrainedVector<bool,ledCols>(ledCols,false)
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
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        {false},
        {true},
        {false}
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
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        {false},
        {false},
        {true},
        {true},
        {false},
        {false}
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
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        {false},
        {false},
        {false},
        {true},
        {true},
        {false},
        {false},
        {false}
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
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        {false,false,false,false,false,false,false,false,false,false,false,false},
        {false,true,true,true,true,true,true,true,true,true,true,false},
        {false,true,false,false,false,false,false,false,false,false,true,false},
        {false,true,false,true,true,true,true,true,true,false,true,false},
        {false,true,false,true,true,true,true,true,true,false,true,false},
        {false,true,false,false,false,false,false,false,false,false,true,false},
        {false,true,true,true,true,true,true,true,true,true,true,false},
        {false,false,false,false,false,false,false,false,false,false,false,false}
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
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        {false,true,false},
        {true,false,false}
    };
    setUpExpected(windowRow,windowCol,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with unflippable matrix failed.");
}