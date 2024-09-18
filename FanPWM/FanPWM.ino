#ifdef CORE_CM4 


  #include "RPC.h"  // comes with the mbed board installation

  #include "Portenta_H7_PWM.h"

  #define pinD6    D6       // PC6  / TIM3_CH1, TIM8_CH1, HRTIM_CHA1, LPTIM3_OUT

  // Can't use myPin with same TIMx. For example, 
  uint32_t myPin  = pinD6;

  float dutyCycle = 100.0f;

  float freq      = 28000.0f;
  int run = 0;

  mbed::PwmOut* pwm   = NULL;
  void setup() {
    randomSeed(analogRead(A0));
    RPC.begin();
    setPWM(pwm, myPin, freq, dutyCycle);
  }

  void runRandom(){
    dutyCycle = random(60, 100);
    setPWM(pwm, myPin, freq, dutyCycle);
    int num = random(1000, 3000);
    // RPC.println(" PWM :" + String(dutyCycle));
    delay(num);

  }


  void loop() {

    unsigned long ms = 20000;

    unsigned long t = millis () ;
    

    RPC.println(" Start Run: " + String(run) );
    while (millis () - t < ms)
      runRandom();

    
    RPC.println(" Finish Run: " + String(run) );
    run += 1;

    setPWM(pwm, myPin, freq, 100);

    delay(5000);
    
  }

#endif  // end all M4 core programming