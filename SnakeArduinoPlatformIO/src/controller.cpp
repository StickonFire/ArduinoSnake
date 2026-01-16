#include <vector>
#include <array>
#include <cstddef>
#include <cstdint>

#include "hardwareValues.hpp"
#include "modelControllerCommUnits.hpp"
#include "controller.hpp"

void ConstrainedVectorToArray::convertConstrainedVector(ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> input, uint8_t output[ledRows][ledCols]){
    int screenMaxX = (ledRows+input.size())/2;
    int screenMinX = screenMaxX-input.size();
    int screenMaxY = (ledCols+input[0].size())/2;
    int screenMinY = screenMaxY-input[0].size();
    for(int i = 0; i < ledRows; i++){
        for(int j = 0; j < ledCols; j++){
            if(i >= screenMinX && i < screenMaxX && j >= screenMinY && j < screenMaxY)
                output[i][j] = ((input[i-screenMinX][j-screenMinY] == Empty) ? 0 : 1);
            else
                output[i][j] = 1;
        }
    }
}

Command ControlsParser::parseButtons(std::array<bool,3> &inputs){
    if(inputs[1])
        return Pause;
    if(!(inputs[0] ^ inputs[2]))
        return Straight;
    if(inputs[0])
        return LeftTurn;
    return RightTurn;
}

ArduinoSnakeController::ArduinoSnakeController(ControlsParser inputParser,ConstrainedVectorToArray outputParser, SnakeModel model):
    inputParser(inputParser), outputParser(outputParser), model(model) { }

void ArduinoSnakeController::moveStep(std::array<bool,3> &inputs,uint8_t outputLocation[ledRows][ledCols]){
    Command cmd = inputParser.parseButtons(inputs);
    model.advanceState(cmd);
    outputParser.convertConstrainedVector(model.getMap(),outputLocation);
}