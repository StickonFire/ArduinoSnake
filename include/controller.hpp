#ifndef CONTROLLER
#define CONTROLLER

#include "hardwareValues.hpp"
#include "modelControllerCommUnits.hpp"
#include "snakeModel.hpp"
/**
 * This class acts as the controller's means of sending View information into the model.
 * It takes inputs captured by the view and converts them into information consumable by the model.
 */
class ControlsParser{
    public:
    /**
     * Given the states of 3 buttons, generates a command.
     */
        Command parseButtons(std::array<bool,3> &inputs);
};

#endif