#include <vector>
#include <array>

#include "hardwareValues.hpp"
#include "modelControllerCommUnits.hpp"
#include "controller.hpp"
Command ControlsParser::parseButtons(std::array<bool,3> &inputs){
    if(inputs[1])
        return Pause;
    if(!(inputs[0] ^ inputs[2]))
        return Straight;
    if(inputs[0])
        return LeftTurn;
    return RightTurn;
}
