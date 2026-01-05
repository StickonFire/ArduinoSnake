#include <array>
#include <string>
#include <map>
#include <gtest/gtest.h>

#include "modelControllerCommUnits.hpp"
#include "snakeModel.hpp"
#include "controller.hpp"

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