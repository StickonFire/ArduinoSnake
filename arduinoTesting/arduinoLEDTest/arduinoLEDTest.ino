#include "Arduino_LED_Matrix.h"
#include <Modulino.h>
#include <Wire.h>

ModulinoButtons buttons;

ArduinoLEDMatrix matrix;
byte frame[8][12];
int currentX;
int currentY;
int xDirection = 1;
int yDirection = 1;

int delayTime = 100;
bool paused = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire1.begin();
  Modulino.begin(Wire1);
  buttons.begin();

  matrix.begin();
  frame[0][0] = 1;
  currentX = 0;
  currentY = 0;
  buttons.setLeds(true, false, false);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(delayTime);
  if(buttons.update()){
    if(buttons.isPressed(0)){
      delayTime = 25;
      buttons.setLeds(true, false, paused);
    }
    if(buttons.isPressed(1)){
      delayTime = 100;
      buttons.setLeds( false, true,paused);
    }
    if(buttons.isPressed(2)){
      paused = !paused;
      buttons.setLeds(false, false, paused);
    }
  }
  if(!paused){
    frame[currentX][currentY] = 0;
    if(currentX + xDirection < 0 || currentX + xDirection > 7){
      xDirection *= -1;
      if(currentY + yDirection < 0 || currentY + yDirection > 11){
        yDirection *= -1;
      }
      currentY += yDirection;
    } 
    else {
      currentX += xDirection;
    }
    frame[currentX][currentY] = 1;
    matrix.renderBitmap(frame,8,12);
  }
}
