#include <Modulino.h>
#include <Wire.h>
#include "Arduino_LED_Matrix.h"

#include <array>

#include "constrainedVector.hpp"
#include "hardwareValues.hpp"
#include "modelControllerCommUnits.hpp"
#include "snakeModel.hpp"
#include "controller.hpp"
#include "randomNumberGenerator.hpp"

ArduinoLEDMatrix matrix;
ModulinoButtons buttons;

uint8_t frame[ledRows][ledCols];
int delayTime = 100;
std::array<bool,3> buttonStates;

SnakeModel model;
ArduinoSnakeController controller(ControlsParser(),ConstrainedVectorToArray(),model);

class arduinoRandom: public randomNumberGenerator {
    public:
    arduinoRandom(){
        randomSeed(analogRead(0));
    }

    int selectNumber(int maxNumber){
        return int(random(maxNumber));
    }
};

arduinoRandom rng;

SnakeModel setUpModel(randomNumberGenerator *rng){
    Snake snake(Coordinate(3,5),Coordinate(3,2),ConstrainedVector<Direction,ledNums>{Right,Right,Right,Right}); 
    ConstrainedVector<ConstrainedVector<MapTileState,ledCols>,ledRows> map(ledRows,ConstrainedVector<MapTileState,ledCols>(ledCols,Empty));
    for(int i = 2; i <= 5; i++){
        map[3][i] = SnakeTile;
    }
    return SnakeModel(map,snake,false,false);
}

void setup(){
    Serial.begin(9600);
    Wire1.begin();
    Modulino.begin(Wire1);
    buttons.begin();
    matrix.begin();
    model = setUpModel();
    controller.setModel(model);
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