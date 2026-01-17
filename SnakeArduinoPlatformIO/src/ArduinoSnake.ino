#include <Modulino.h>
#include <Wire.h>
#include "Arduino_LED_Matrix.h"

#include <array>

#include "constrainedVector.hpp"
#include "hardwareValues.hpp"
#include "modelControllerCommUnits.hpp"
#include "snakeModel.hpp"
#include "controller.hpp"

ArduinoLEDMatrix matrix;
ModulinoButtons buttons;

ArduinoSnakeController controller(ControlsParser(),ConstrainedVectorToArray(),
    SnakeModel(ledRows,ledCols,Coordinate(ledRows/2,ledCols/2),Right,true));
uint8_t frame[ledRows][ledCols];
int delayTime = 125;
std::array<bool,3> buttonStates;

void setup(){
    Serial.begin(9600);
    Wire1.begin();
    Modulino.begin(Wire1);
    buttons.begin();
    matrix.begin();

    buttons.setLeds(false, false, false);
}

void loop(){
    delay(delayTime);
    if(buttons.update()){
        buttonStates[0] = buttons.isPressed(0);
        buttonStates[1] = buttons.isPressed(1);
        buttonStates[2] = buttons.isPressed(2);

        buttons.setLeds(buttonStates[0],buttonStates[1],buttonStates[2]);
    }
    controller.moveStep(buttonStates,frame);
    matrix.renderBitmap(frame,ledRows,ledCols);
}