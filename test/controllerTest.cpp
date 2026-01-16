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

        void setUpExpected(int startWidth, int startHeight, ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> init){
            for(int i = startWidth; i < init.size(); i++){
                for(int j = startHeight; j < init[0].size(); j++){
                    expected[i+startWidth][j+startHeight] = init[i][j];
                }
            }
        }

        void checkEquality(std::string message){
            for(int i = 0; i < ledRows; i++){
                for(int j = 0; j < ledCols; j++){
                    EXPECT_EQ(result[i][j],expected[i][j]) << message << "Location: [" << i << "][" << j << "]\n";
                }
            }
        }
};

TEST_F(ConstrainedVectorToArrayTest,OddWidth){
    int windowX = 4;
    int windowY = 3;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty,SnakeTile,Empty},
    };
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        {false,true,false}
    };
    setUpExpected(windowX,windowY,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with oddWidth failed.");
}

TEST_F(ConstrainedVectorToArrayTest,EvenWidth){
    int windowX = 5;
    int windowY = 3;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty,Empty},
    };
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        {false,false}
    };
    setUpExpected(windowX,windowY,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with evenWidth failed.");
}

TEST_F(ConstrainedVectorToArrayTest,FullWidth){
    int windowX = 0;
    int windowY = 3;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        ConstrainedVector<MapTileState,ledCols>(ledCols,Empty)
    };
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        ConstrainedVector<bool,ledCols>(ledCols,false)
    };
    setUpExpected(windowX,windowY,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with FullWidth failed.");
}

TEST_F(ConstrainedVectorToArrayTest,OddHeight){
    int windowX = 5;
    int windowY = 2;
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
    setUpExpected(windowX,windowY,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with OddHeight failed.");
}

TEST_F(ConstrainedVectorToArrayTest,EvenHeight){
    int windowX = 5;
    int windowY = 1;
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
    setUpExpected(windowX,windowY,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with EvenHeight failed.");
}

TEST_F(ConstrainedVectorToArrayTest,FullHeight){
    int windowX = 5;
    int windowY = 0;
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
    setUpExpected(windowX,windowY,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with FullHeight failed.");
}

TEST_F(ConstrainedVectorToArrayTest,FullScreen){
    int windowX = 0;
    int windowY = 0;
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
    setUpExpected(windowX,windowY,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with FullScreen failed.");
}

TEST_F(ConstrainedVectorToArrayTest,InsideCheck){
    int windowX = 2;
    int windowY = 5;
    input = ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows>{
        {Empty,SnakeTile,Empty},
        {SnakeTile,Empty,Empty}
    };
    ConstrainedVector<ConstrainedVector<bool,ledCols>,ledRows> initExpected{
        {false,true,false},
        {true,false,false}
    };
    setUpExpected(windowX,windowY,initExpected);
    test.convertConstrainedVector(input,result);
    checkEquality("Test with unflippable matrix failed.");
}