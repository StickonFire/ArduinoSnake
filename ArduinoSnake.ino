#include <Modulino.h>
#include <Wire.h>
#include "Arduino_LED_Matrix.h"

#include "include/constrainedVector.hpp"
#include "include/hardwareValues.hpp"
#include "include/modelControllerCommUnits.hpp"
#include "include/snakeModel.hpp"
#include "include/controller.hpp"

ArduinoLEDMatrix matrix;
ModulinoButtons buttons;

ArduinoSnakeController controller(ControlsParser(),ConstrainedVectorToArray(),
  SnakeModel(ledRows,ledCols,Coordinate(ledRows/2,ledCols/2),Right));
byte frame[8][12];

void setup(){

}

void loop(){

}