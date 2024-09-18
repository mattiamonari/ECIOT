/****************************************************************************************************************************
  Portenta_H7_PWM_Impl.h
  For Portenta_H7 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Portenta_H7_PWM
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one Portenta_H7 STM32 timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 2.0.2

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      21/09/2021 Initial coding for Portenta_H7 using ArduinoCore-mbed mbed_portenta core
  2.0.0   K.Hoang      10/12/2021 Use new library code and examples
  2.0.1   K.Hoang      11/12/2021 Fix PWM_Multi example. Temporary fix polarity for HRTIM PWM
  2.0.2   K.Hoang      07/02/2022 Convert to h-only. Optimize code.
*****************************************************************************************************************************/

#ifndef Portenta_H7_PWM_IMPL_H
#define Portenta_H7_PWM_IMPL_H

#ifndef _PWM_LOGLEVEL_
  #define _PWM_LOGLEVEL_       1
#endif

#include "Arduino.h"
#include "Portenta_H7_PWM.h"

#include "PWM_Generic_Debug.h"


bool isValidPWMPin(const pin_size_t& pin)
{
  if ( (pin == D0) || (pin == D1) || (pin == D2) || (pin == D3) || (pin == D4) || (pin == D5) || (pin == D6) )
  {
    return true;
  }
  
  PWM_LOGERROR1("Not PWM pin = ", pin);
  
  return false;
}

bool isValidPWMDutyCycle(const pin_size_t& pin, const float& dutyCycle)
{
  if ( (dutyCycle < 0.0f) || (dutyCycle > 100.0f) )
  {
    PWM_LOGERROR3("Bad dutyCycle = ", dutyCycle, ", pin = ", pin);
    
    return false;
  }

  return true;
}

bool isUsingHRTIM(const pin_size_t& pin)
{
  if ( (pin == D3) || (pin == D6) )
  {
    PWM_LOGDEBUG1("Using HRTIM pin = ", pin);
    
    return true;
  }

  return false;
}

// Will calculate accurate min freq if necessary, depending on Portenta Clock
#define MIN_HRTIM_PWM_FREQ      800

// Check if freq > 0 and if HRTIM => freq >= MIN_HRTIM_PWM_FREQ
bool isValidPWMFreq(const pin_size_t& pin, const float& frequency)
{
  if ( (frequency <= 0) || ( isUsingHRTIM(pin) && ( frequency < MIN_HRTIM_PWM_FREQ ) ) )
  {
    if (isUsingHRTIM(pin))
    {
      PWM_LOGERROR5("Bad HRTIM Freq = ", frequency, ", pin = ", pin, ", must be >= ", MIN_HRTIM_PWM_FREQ);
    }
    else
    {
      PWM_LOGERROR3("Bad Freq = ", frequency, ", pin = ", pin);
    }
    
    return false;
  }
  
  return true;
}

bool isValidPWMSettings(const pin_size_t& pin, const float& frequency, const float& dutyCycle)
{
  if ( (frequency <= 0) || ( isUsingHRTIM(pin) && ( frequency < MIN_HRTIM_PWM_FREQ ) ) )
  {
    if (isUsingHRTIM(pin))
    {
      PWM_LOGERROR5("Bad HRTIM Freq = ", frequency, ", pin = ", pin, ", must be >= ", MIN_HRTIM_PWM_FREQ);
    }
    else
    {
      PWM_LOGERROR3("Bad Freq = ", frequency, ", pin = ", pin);
    }
    
    return false;
  } 
  else if ( (dutyCycle < 0.0f) || (dutyCycle > 100.0f) )
  {
    PWM_LOGERROR3("Bad dutyCycle = ", dutyCycle, ", pin = ", pin);
    
    return false;
  }
  else if ( !( (pin == D0) || (pin == D1) || (pin == D2) || (pin == D3) || (pin == D4) || (pin == D5) || (pin == D6) ) )
  {
    return false;
  }
  
  return true;
}

// dutyCycle from 0.0f to 100.0f
mbed::PwmOut* setPWM(mbed::PwmOut* &pwm, const pin_size_t& pin, const float& frequency, const float& dutyCycle)
{
  PWM_LOGDEBUG7("Freq = ", frequency, ", \tDutyCycle = ", dutyCycle, ", \tDutyCycle % = ", dutyCycle / 100, ", \tPin = ", pin);
    
  //if ( !isValidPWMPin(pin) || !isValidPWMFreq(pin, frequency) || !isValidPWMDutyCycle(pin, dutyCycle) )
  if ( !isValidPWMSettings(pin, frequency, dutyCycle) )
  {
    //PWM_LOGERROR("Bad pin, Freq or dutyCycle");
    
    return NULL;
  }

  float percent = (float) dutyCycle / 100;
  
  if (digitalPinToPwm(pin) == NULL)
  {
    PWM_LOGDEBUG("New pwm");
    
    pwm = new mbed::PwmOut(digitalPinToPinName(pin));
    
    digitalPinToPwm(pin) = pwm;

    pwm->period_us( 1000000.0f/frequency );  

    pwm->write(percent);
  }
  else if (pwm && (digitalPinToPwm(pin) == pwm) )
  {
    PWM_LOGDEBUG("Use existing pwm");
    
    pwm->period_us( 1000000.0f/frequency );  

    pwm->write(percent);
  }
  
  return pwm;
}

mbed::PwmOut* stopPWM(mbed::PwmOut* &pwm, const pin_size_t& pin)
{
  if (isUsingHRTIM(pin))
  {
    // Check https://github.com/arduino/ArduinoCore-mbed/issues/352
    // Will remove when the bug is fixed
    return setPWM(pwm, pin, 1000.0f, 100.0f);
  }
  else
  {
    return setPWM(pwm, pin, 1000.0f, 0.0f);
  }
}

#endif    // Portenta_H7_PWM_IMPL_H


