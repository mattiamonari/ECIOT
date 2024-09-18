#include <Wire.h>


void setup()
{
  Wire.begin(); // Initiate the Wire library
  Serial.begin(9600);
  delay(100);

  Wire.beginTransmission(0x70);
  Wire.write(2);  
  Wire.endTransmission();
  
}

void loop()
{
  Wire.beginTransmission(0x70);
  Wire.requestFrom(0x70, 8);

  while(Wire.available()>0) {  // 
    long X0 = Wire.read(); // Reads the data from the register
    if(X0 != 0)
      Serial.println(X0);
  }
  delay(1000);
}