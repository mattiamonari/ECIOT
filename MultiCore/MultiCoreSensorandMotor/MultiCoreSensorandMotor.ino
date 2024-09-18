// #include <Adafruit_VCNL4040.h>
// #include <Wire.h>
// #include "protothreads.h"
#ifdef CORE_CM7


  #include "RPC.h"  // comes with the mbed board installation
  #include <Adafruit_VCNL4040.h>
  #include "Adafruit_VL53L0X.h"
  #include <Wire.h>
  Adafruit_VCNL4040 vcnl4040 = Adafruit_VCNL4040();
  #define VL53L0X_I2C_ADDR 0x29 ///< Default sensor I2C address


  int sensor = 0;

  int myLED; 


  // Set an M7 core global variable
  int myIntGlobal = 1234;

  void Multiplex(uint8_t bus){
    Wire.beginTransmission(0x70);  // TCA9548A address
    Wire.write(1 << bus);          // send byte to select bus
    Wire.endTransmission();
    // Serial.print(bus);
  }
  

  int setVar(int a) {
    myIntGlobal = (int)a;
    return a;
  }

  void getSensorData() {
    sensor = (sensor + 1) % 3;
    Multiplex(sensor);
    // Serial.print("Sensor number:");Serial.println(sensor);
    
    // Serial.print("Proximity:"); Serial.println(vcnl4040.getProximity());
    // Serial.print("Ambient light:"); Serial.println(vcnl4040.getLux());
    // Serial.print("Raw white light:"); Serial.println(vcnl4040.getWhiteLight());
    // Serial.println("");
    

    Serial.print(sensor);
    Serial.print(",");
    Serial.print(vcnl4040.getProximity());
    Serial.print(",");
    Serial.print(vcnl4040.getLux());
    Serial.print(",");
    Serial.print(vcnl4040.getWhiteLight());
    Serial.println();
    
  
    // delay(500);
}

void setup() {
   randomSeed(analogRead(A0));
   RPC.begin();
    
   bootM4();  
   myLED = LEDB; // on-board blue LED
   Serial.begin(115200);

   RPC.bind("setVar", setVar); // do these have to be the same? 
   
   pinMode(myLED, OUTPUT);
  //  Serial.begin(115200);
    // Wait until serial port is opened
    // Start I2C communication with the Multiplexer
    Wire.begin();

    // Init OLED display on bus number 2
    Multiplex(0);
    delay(500);
    Multiplex(1);
    delay(500);
    Multiplex(2);
    delay(500);

    while (!Serial) { delay(1); }

    // Serial.println("Adafruit VCNL4040 Config demo");

    if (!vcnl4040.begin()) {
      // Serial.println("Couldn't find VCNL4040 chip");
      while (1);
    }
    // Serial.println("Found VCNL4040 chip");

    // vcnl4040.setProximityLEDCurrent(VCNL4040_LED_CURRENT_200MA);
    // Serial.print("Proximity LED current set to: ");
    switch(vcnl4040.getProximityLEDCurrent()) {
      case VCNL4040_LED_CURRENT_50MA: Serial.println("50 mA"); break;
      case VCNL4040_LED_CURRENT_75MA: Serial.println("75 mA"); break;
      case VCNL4040_LED_CURRENT_100MA: Serial.println("100 mA"); break;
      case VCNL4040_LED_CURRENT_120MA: Serial.println("120 mA"); break;
      case VCNL4040_LED_CURRENT_140MA: Serial.println("140 mA"); break;
      case VCNL4040_LED_CURRENT_160MA: Serial.println("160 mA"); break;
      case VCNL4040_LED_CURRENT_180MA: Serial.println("180 mA"); break;
      case VCNL4040_LED_CURRENT_200MA: Serial.println("200 mA"); break;
    }
    
    //vcnl4040.setProximityLEDDutyCycle(VCNL4040_LED_DUTY_1_40);
    // Serial.print("Proximity LED duty cycle set to: ");
    switch(vcnl4040.getProximityLEDDutyCycle()) {
      case VCNL4040_LED_DUTY_1_40: Serial.println("1/40"); break;
      case VCNL4040_LED_DUTY_1_80: Serial.println("1/80"); break;
      case VCNL4040_LED_DUTY_1_160: Serial.println("1/160"); break;
      case VCNL4040_LED_DUTY_1_320: Serial.println("1/320"); break;
    }

    //vcnl4040.setAmbientIntegrationTime(VCNL4040_AMBIENT_INTEGRATION_TIME_80MS);
    // Serial.print("Ambient light integration time set to: ");
    switch(vcnl4040.getAmbientIntegrationTime()) {
      case VCNL4040_AMBIENT_INTEGRATION_TIME_80MS: Serial.println("80 ms"); break;
      case VCNL4040_AMBIENT_INTEGRATION_TIME_160MS: Serial.println("160 ms"); break;
      case VCNL4040_AMBIENT_INTEGRATION_TIME_320MS: Serial.println("320 ms"); break;
      case VCNL4040_AMBIENT_INTEGRATION_TIME_640MS: Serial.println("640 ms"); break;
    }


    //vcnl4040.setProximityIntegrationTime(VCNL4040_PROXIMITY_INTEGRATION_TIME_8T);
    // Serial.print("Proximity integration time set to: ");
    switch(vcnl4040.getProximityIntegrationTime()) {
      case VCNL4040_PROXIMITY_INTEGRATION_TIME_1T: Serial.println("1T"); break;
      case VCNL4040_PROXIMITY_INTEGRATION_TIME_1_5T: Serial.println("1.5T"); break;
      case VCNL4040_PROXIMITY_INTEGRATION_TIME_2T: Serial.println("2T"); break;
      case VCNL4040_PROXIMITY_INTEGRATION_TIME_2_5T: Serial.println("2.5T"); break;
      case VCNL4040_PROXIMITY_INTEGRATION_TIME_3T: Serial.println("3T"); break;
      case VCNL4040_PROXIMITY_INTEGRATION_TIME_3_5T: Serial.println("3.5T"); break;
      case VCNL4040_PROXIMITY_INTEGRATION_TIME_4T: Serial.println("4T"); break;
      case VCNL4040_PROXIMITY_INTEGRATION_TIME_8T: Serial.println("8T"); break;
    }

    //vcnl4040.setProximityHighResolution(false);
    // Serial.print("Proximity measurement high resolution? ");
    // Serial.println(vcnl4040.getProximityHighResolution() ? "True" : "False");

    // Serial.println("");

}

void loop() {
   VL53L0X_RangingMeasurementData_t measure;
  // Serial.println("---------------------------------");
  // Serial.println("From M7 showing global variable: "+ String(myIntGlobal));
   while (RPC.available()) {
     digitalWrite(myLED, LOW); // turn the Portenta on-board LED on 
     Serial.write(RPC.read()); // check if the M4 has sent an RPC println
   }  
  //  delay(100); // minor slow down just for fun
   digitalWrite(myLED, HIGH); // turn the Portenta on-board LED off

    getSensorData();
    getSensorData();
    getSensorData();
    // Serial.print(measure.RangeMilliMeter);
    // Serial.println();

    delay(100);
}


#endif   // end al M7 core programming




