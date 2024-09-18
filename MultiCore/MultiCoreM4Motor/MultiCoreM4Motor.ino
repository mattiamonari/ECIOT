
#ifdef CORE_CM4 

  #include "RPC.h"  // comes with the mbed board installation

  #include <Stepper.h>
  #include "Arduino.h"

  #define MAX_POS_FORWARD 1000
  #define MAX_POS_BACKWARD -1000

  int myLED; 
  const int stepsPerRevolution = 2038;

  long position = 0;
  short steps = 0;
  int run = 0;

  // the setup function runs once when you press reset or power the board
  Stepper myStepper = Stepper(stepsPerRevolution, 7,9,8,10);

void setup() {
   randomSeed(analogRead(A0));
   RPC.begin();
   myLED = LEDG; // on-board greeen LED
   
   pinMode(myLED, OUTPUT);
   myStepper.setSpeed(10);
   myStepper.step(1000);
   position = 1000;
}

void runRandom() {
bool dir = random(0, 2);

  steps = random(50, 1000);
  
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


  // Serial.print("Object position: \t");
  // Serial.println(position);


	delay(1000);

}


void loop() {

  //  digitalWrite(myLED, LOW); // turn the Portenta on-board LED on 
  //  delay(200); // wait for a second
  //  digitalWrite(myLED, HIGH); // turn the Portenta on-board LED off 

  //  int myRand = rand() % 100;  // from 0 to 99
  //  auto res = RPC.call("setVar", myRand).as<int>();
  //  RPC.println("From M4 core setting M7 variable to: " + String(res));
  RPC.println(" Start Run: " + String(run) );

  unsigned long ms = 20000;

  unsigned long t = millis () ;
  while (millis () - t < ms)
    runRandom();

  // RPC.println("Pose" + String(position));
  if (position >=0) {
    RPC.println(" Class :" + String(0));
  }
  else {
    RPC.println(" Class :" + String(1));
  }

  int initialPose = 1000 - position;
  myStepper.step(initialPose);
  position = 1000;
  run += 1;

	delay(2000);

                  
}

#endif  // end all M4 core programming
