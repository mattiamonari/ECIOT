
#include <Stepper.h>
#include "Arduino.h"

#define MAX_POS_FORWARD 500
#define MAX_POS_BACKWARD -500

const int stepsPerRevolution = 2038;

long position = 0;
short steps = 0;

// the setup function runs once when you press reset or power the board
Stepper myStepper = Stepper(stepsPerRevolution, 7,9,8,10);

void setup() {
  myStepper.setSpeed(5);
}

// the loop function runs over and over again forever
void loop() {

  bool dir = random(0, 2);

  steps = random(50, 100);
  
    if(dir)
    {
      if(position + steps < MAX_POS_FORWARD)
      {
         myStepper.step(steps);
         position += steps;
      }
      else 
      {
          myStepper.step(MAX_POS_FORWARD - position);
          position = MAX_POS_FORWARD;
      }
    }
    else   
    {
      if (position - steps > MAX_POS_BACKWARD)
      {
          myStepper.step(-steps);
          position -= steps;
      }
      else 
      {
          myStepper.step(MAX_POS_BACKWARD - position);
          position = MAX_POS_BACKWARD;
      }
    }


  Serial.print("Object position: \t");
  Serial.println(position);

	delay(1000);
}