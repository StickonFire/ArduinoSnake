#ifndef CONTROLLER
#define CONTROLLER
#include <array>
#include <cstddef>
#include <cstdint>

#include "hardwareValues.hpp"
#include "modelControllerCommUnits.hpp"
#include "snakeModel.hpp"

/**
 * This conversion class holds a simple conversion function to create an arduinoLEDMatrix readable array out of the expected output of snakeModel.
 */
class ConstrainedVectorToArray{
    public:
        /**
         * Given a 2D CosntrainedVector, convert it into a bool array readable by a arduino LED Matrix.
         */
        void convertConstrainedVector(ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> input, uint8_t output[ledRows][ledCols]);
};

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

class ArduinoSnakeController{
    ControlsParser inputParser;
    ConstrainedVectorToArray outputParser;
    SnakeModel model;

    public:
        ArduinoSnakeController(ControlsParser inputParser,ConstrainedVectorToArray outputParser, SnakeModel model);
        void moveStep(std::array<bool,3> &inputs, uint8_t outputLocation[ledRows][ledCols]);
};

#endif