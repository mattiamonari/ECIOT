#include "Adafruit_VL53L0X.h"
#include "Arduino.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// the setup function runs once when you press reset or power the board
void setup() {

    
    //IF WE WANT TO TRY MULTITHREADING
    //bootM4();

    Serial.println("Adafruit VL53L0X test");
    if (!lox.begin()) {
      Serial.println(F("Failed to boot VL53L0X"));
      while(1);
    }
    // power 
    Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

// the loop function runs over and over again forever
void loop() {
    VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100);
}