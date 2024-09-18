# Portenta_H7_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/Portenta_H7_PWM.svg?)](https://www.ardu-badge.com/Portenta_H7_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/Portenta_H7_PWM.svg)](https://github.com/khoih-prog/Portenta_H7_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/Portenta_H7_PWM/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/Portenta_H7_PWM.svg)](http://github.com/khoih-prog/Portenta_H7_PWM/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>

---
---

## Table of Contents

* [Important Change from v2.0.2](#Important-Change-from-v202)
* [Why do we need this Portenta_H7_PWM library](#why-do-we-need-this-Portenta_H7_PWM-library)
  * [Features](#features)
  * [Why using hardware-based PWM is better](#why-using-hardware-based-pwm-is-better)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error) 
* [Packages' Patches](#packages-patches)
  * [1. For Portenta_H7 boards using Arduino IDE in Linux](#1-for-portenta_h7-boards-using-arduino-ide-in-linux)
* [More useful Information about STM32 Timers](#more-useful-information-about-stm32-timers)
* [Available Timers for Portenta_H7](#available-timers-for-portenta_h7)
* [Portenta_H7 Timer and pin association](#Portenta_H7-Timer-and-pin-association)
* [Usage](#usage)
  * [1. Prepare PWM settings](#1-Prepare-PWM-settings)
  * [2. Set PWM Frequency and dutycycle](#2-Set-PWM-Frequency-and-dutycycle)
* [Examples](#examples)
  * [ 1. PWM_Multi](examples/PWM_Multi)
  * [ 2. PWM_Single](examples/PWM_Single)
  * [ 3. multiFileProject](examples/multiFileProject). **New**
* [Example PWM_Multi](#example-PWM_Multi)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. PWM_Single on PORTENTA_H7_M7](#1-PWM_Single-on-PORTENTA_H7_M7)
  * [2. PWM_Multi on PORTENTA_H7_M7](#2-PWM_Multi-on-PORTENTA_H7_M7)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Important Change from v2.0.2

From v2.0.2, `h-only` style is used for this library.
Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)

---
---

### Why do we need this [Portenta_H7_PWM library](https://github.com/khoih-prog/Portenta_H7_PWM)

### Features

This library enables you to use Hardware Timers on an STM32H7-based Portenta_H7 board to create and output PWM to pins. 

---

The most important feature is they're purely hardware-based PWM channels. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

This important feature is absolutely necessary for mission-critical tasks. These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more precise (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

The [**PWM_Multi**](examples/PWM_Multi) will demonstrate the usage of multichannel PWM using multiple Hardware Timers. The 4 independent Hardware Timers are used **to control 4 different PWM outputs**, with totally independent frequencies and dutycycles. You can **start, stop, change and restore the settings of any PWM channel on-the-fly**.

Being hardware-based PWM, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet or Blynk services.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

---

#### Why using hardware-based PWM is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

1. **Portenta_H7 boards** such as Portenta_H7 Rev2 ABX00042, etc., using [**ArduinoCore-mbed mbed_portenta** core](https://github.com/arduino/ArduinoCore-mbed)

---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`ArduinoCore-mbed mbed_portenta core 2.7.2+`](https://github.com/arduino/ArduinoCore-mbed) for Arduino **Portenta_H7** boards, such as **Portenta_H7 Rev2 ABX00042, etc.**. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-mbed.svg)](https://github.com/arduino/ArduinoCore-mbed/releases/latest)

---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**Portenta_H7_PWM**](https://github.com/khoih-prog/Portenta_H7_PWM), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/Portenta_H7_PWM.svg?)](https://www.ardu-badge.com/Portenta_H7_PWM) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**Portenta_H7_PWM**](https://github.com/khoih-prog/Portenta_H7_PWM) page.
2. Download the latest release `Portenta_H7_PWM-master.zip`.
3. Extract the zip file to `Portenta_H7_PWM-master` directory 
4. Copy whole `Portenta_H7_PWM-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**Portenta_H7_PWM** library](https://platformio.org/lib/show/12853/Portenta_H7_PWM) by using [Library Manager](https://platformio.org/lib/show/12853/Portenta_H7_PWM/installation). Search for **Portenta_H7_PWM** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include this `.hpp` file

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "Portenta_H7_PWM.hpp"    //https://github.com/khoih-prog/Portenta_H7_PWM
```

in many files. But be sure to use the following `.h` file **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "Portenta_H7_PWM.h"      //https://github.com/khoih-prog/Portenta_H7_PWM
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.


---
---

### Packages' Patches

#### 1. For Portenta_H7 boards using Arduino IDE in Linux

  **To be able to upload firmware to Portenta_H7 using Arduino IDE in Linux (Ubuntu, etc.)**, you have to copy the file [portenta_post_install.sh](Packages_Patches/arduino/hardware/mbed_portenta/2.7.2/portenta_post_install.sh) into mbed_portenta directory (~/.arduino15/packages/arduino/hardware/mbed_portenta/2.7.2/portenta_post_install.sh). 
  
  Then run the following command using `sudo`
  
```
$ cd ~/.arduino15/packages/arduino/hardware/mbed_portenta/2.7.2
$ chmod 755 portenta_post_install.sh
$ sudo ./portenta_post_install.sh
```

This will create the file `/etc/udev/rules.d/49-portenta_h7.rules` as follows:

```
# Portenta H7 bootloader mode UDEV rules

SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="035b", GROUP="plugdev", MODE="0666"
```

Supposing the ArduinoCore-mbed core version is 2.7.2. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/2.7.2/portenta_post_install.sh`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/x.yy.zz/portenta_post_install.sh`


---
---


## More useful Information about STM32 Timers

The Timers of STM32s are numerous, yet very sophisticated and powerful.

In general, across the STM32 microcontrollers families, the timer peripherals that have the same name also have the same features set, but there are a few exceptions. 

The general purpose timers embedded by the STM32 microcontrollers share the same backbone structure; they differ only on the level of features embedded by a given timer peripheral. 

The level of features integration for a given timer peripheral is decided based on the applications field that it targets.

The timer peripherals can be classified as:

• Advanced-configuration timers like TIM1 and TIM8 among others.
• General-purpose configuration timers like TIM2 and TIM3 among others
• Lite-configuration timers like TIM9, TIM10, TIM12 and TIM16 among others
• Basic-configuration timers like TIM6 and TIM7 among others.


More information can be found at [**Embedded-Lab STM32 TIMERS**](http://embedded-lab.com/blog/stm32-timers/)

To be sure which Timer is available for the board you're using, check the Core Package's related files. For example, for **Portenta_H7 using STM32H747XI**, check this file:

1. `~/.arduino15/packages/STM32/hardware/stm32/2.0.0/system/Drivers/CMSIS/Device/ST/STM32H7xx/Include/stm32h7xx.h`


The information will be as follows:

```
typedef struct
{
  __IO uint32_t CR1;         /*!< TIM control register 1,                   Address offset: 0x00 */
  __IO uint32_t CR2;         /*!< TIM control register 2,                   Address offset: 0x04 */
  __IO uint32_t SMCR;        /*!< TIM slave mode control register,          Address offset: 0x08 */
  __IO uint32_t DIER;        /*!< TIM DMA/interrupt enable register,        Address offset: 0x0C */
  __IO uint32_t SR;          /*!< TIM status register,                      Address offset: 0x10 */
  __IO uint32_t EGR;         /*!< TIM event generation register,            Address offset: 0x14 */
  __IO uint32_t CCMR1;       /*!< TIM capture/compare mode register 1,      Address offset: 0x18 */
  __IO uint32_t CCMR2;       /*!< TIM capture/compare mode register 2,      Address offset: 0x1C */
  __IO uint32_t CCER;        /*!< TIM capture/compare enable register,      Address offset: 0x20 */
  __IO uint32_t CNT;         /*!< TIM counter register,                     Address offset: 0x24 */
  __IO uint32_t PSC;         /*!< TIM prescaler,                            Address offset: 0x28 */
  __IO uint32_t ARR;         /*!< TIM auto-reload register,                 Address offset: 0x2C */
  __IO uint32_t RCR;         /*!< TIM repetition counter register,          Address offset: 0x30 */
  __IO uint32_t CCR1;        /*!< TIM capture/compare register 1,           Address offset: 0x34 */
  __IO uint32_t CCR2;        /*!< TIM capture/compare register 2,           Address offset: 0x38 */
  __IO uint32_t CCR3;        /*!< TIM capture/compare register 3,           Address offset: 0x3C */
  __IO uint32_t CCR4;        /*!< TIM capture/compare register 4,           Address offset: 0x40 */
  __IO uint32_t BDTR;        /*!< TIM break and dead-time register,         Address offset: 0x44 */
  __IO uint32_t DCR;         /*!< TIM DMA control register,                 Address offset: 0x48 */
  __IO uint32_t DMAR;        /*!< TIM DMA address for full transfer,        Address offset: 0x4C */
  uint32_t      RESERVED1;   /*!< Reserved, 0x50                                                 */
  __IO uint32_t CCMR3;       /*!< TIM capture/compare mode register 3,      Address offset: 0x54 */
  __IO uint32_t CCR5;        /*!< TIM capture/compare register5,            Address offset: 0x58 */
  __IO uint32_t CCR6;        /*!< TIM capture/compare register6,            Address offset: 0x5C */
  __IO uint32_t AF1;         /*!< TIM alternate function option register 1, Address offset: 0x60 */
  __IO uint32_t AF2;         /*!< TIM alternate function option register 2, Address offset: 0x64 */
  __IO uint32_t TISEL;       /*!< TIM Input Selection register,             Address offset: 0x68 */
} TIM_TypeDef;
```

and

```
#define PERIPH_BASE            0x40000000UL /*!< Base address of : AHB/ABP Peripherals   
/*!< Peripheral memory map */
#define APB1PERIPH_BASE        PERIPH_BASE

/*!< APB1 peripherals */
/*!< D2_APB1PERIPH peripherals */
#define TIM2_BASE             (D2_APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (D2_APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (D2_APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE             (D2_APB1PERIPH_BASE + 0x0C00UL)
#define TIM6_BASE             (D2_APB1PERIPH_BASE + 0x1000UL)
#define TIM7_BASE             (D2_APB1PERIPH_BASE + 0x1400UL)
#define TIM12_BASE            (D2_APB1PERIPH_BASE + 0x1800UL)
#define TIM13_BASE            (D2_APB1PERIPH_BASE + 0x1C00UL)
#define TIM14_BASE            (D2_APB1PERIPH_BASE + 0x2000UL)

/*!< APB2 peripherals */
#define TIM1_BASE             (D2_APB2PERIPH_BASE + 0x0000UL)
#define TIM8_BASE             (D2_APB2PERIPH_BASE + 0x0400UL)
...
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000UL)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800UL)
...
#define TI15_BASE            (D2_APB2PERIPH_BASE + 0x4000UL)
#define TIM16_BASE            (D2_APB2PERIPH_BASE + 0x4400UL)
#define TIM17_BASE            (D2_APB2PERIPH_BASE + 0x4800UL)
...
#define HRTIM1_BASE           (D2_APB2PERIPH_BASE + 0x7400UL)
#define HRTIM1_TIMA_BASE      (HRTIM1_BASE + 0x00000080UL)
#define HRTIM1_TIMB_BASE      (HRTIM1_BASE + 0x00000100UL)
#define HRTIM1_TIMC_BASE      (HRTIM1_BASE + 0x00000180UL)
#define HRTIM1_TIMD_BASE      (HRTIM1_BASE + 0x00000200UL)
#define HRTIM1_TIME_BASE      (HRTIM1_BASE + 0x00000280UL)
#define HRTIM1_COMMON_BASE    (HRTIM1_BASE + 0x00000380UL)
...
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define TIM13               ((TIM_TypeDef *) TIM13_BASE)
#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
...
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
...
#define TIM12               ((TIM_TypeDef *) TIM12_BASE)
#define TIM15               ((TIM_TypeDef *) TIM15_BASE)
#define TIM16               ((TIM_TypeDef *) TIM16_BASE)
#define TIM17               ((TIM_TypeDef *) TIM17_BASE)
...
#define HRTIM1              ((HRTIM_TypeDef *) HRTIM1_BASE)
#define HRTIM1_TIMA         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMA_BASE)
#define HRTIM1_TIMB         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMB_BASE)
#define HRTIM1_TIMC         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMC_BASE)
#define HRTIM1_TIMD         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMD_BASE)
#define HRTIM1_TIME         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIME_BASE)
#define HRTIM1_COMMON       ((HRTIM_Common_TypeDef *) HRTIM1_COMMON_BASE)
```

---

## Available Timers for Portenta_H7

This is the temporary list for Portenta_H7 Timers which can be used. The available Timers certainly depends on they are being used for other purpose (core, application, libraries, etc.) or not. You have to exhausively test yourself to be sure.

#### 1. OK to use

**TIM1, TIM4, TIM7, TIM8, TIM12, TIM13, TIM14, TIM15, TIM16, TIM17**

#### 2. Not exist

**TIM9, TIM10, TIM11. Only for STM32F2, STM32F4 or STM32L1**

#### 3.Not declared

**TIM18, TIM19, TIM20, TIM21, TIM22**

#### 3. Not OK => conflict or crash

**TIM2, TIM3, TIM5, TIM6**

---

## Portenta_H7 Timer and pin association

To know which Timer (TIMx) is used for which pin, check [Table 7. STM32H747xI/G pin/ball definition, page 67](https://www.st.com/resource/en/datasheet/stm32h747xi.pdf)

The result for PWM-enable pins can be summarized as follows

```
#define pinD0    D0       // PH15 / TIM8_CH3N
#define pinD1    D1       // PK1  / TIM1_CH1, TIM8_CH3
#define pinD2    D2       // PJ11 / TIM1_CH2, TIM8_CH2N
#define pinD3    D3       // PG7  / HRTIM_CHE2
#define pinD4    D4       // PC7  / TIM3_CH2, TIM8_CH2, HRTIM_CHA2
#define pinD5    D5       // PC6  / TIM3_CH1, TIM8_CH1, HRTIM_CHA1, LPTIM3_OUT
#define pinD6    D6       // PA8  / HRTIM_CHB2 (TIM1_CH1, TIM8_BKIN2)
```

---
---

## Usage

Before using any Timer for a PWM channel, you have to make sure the Timer has not been used by any other purpose.

### 1. Prepare PWM settings

```
// Can't use same TimerIndex again, e.g., the D1 and D2, using TIM1, can't be use concurrently
// That's why D0, D1, D3, D4 and D6 (using TimerIndex 8, 1, HRTIM and 3) are OK together

// Only OK for D0, D1, D2, D4 and D5, PA_0C(D15/A0), PA_1C(D16/A1), 
// D3, D6, D7, D8, D9, D10, D11, D12, D13, D14, D17(PC_2C/A2), D18(PC_3C/3), PC2(D19/A4) LEDG, LEDB not OK
#define pinD0    D0       // PH15 / TIM8_CH3N
#define pinD1    D1       // PK1  / TIM1_CH1, TIM8_CH3
#define pinD2    D2       // PJ11 / TIM1_CH2, TIM8_CH2N
#define pinD3    D3       // PG7  / HRTIM_CHE2
#define pinD4    D4       // PC7  / TIM3_CH2, TIM8_CH2, HRTIM_CHA2
#define pinD5    D5       // PC6  / TIM3_CH1, TIM8_CH1, HRTIM_CHA1, LPTIM3_OUT
#define pinD6    D6       // PA8  / HRTIM_CHB2 (TIM1_CH1, TIM8_BKIN2)

// See https://www.st.com/resource/en/datasheet/stm32h747xi.pdf, Table 7, page 53
// Can't use myPin with same TIMx. For example, 
// pinD1 and pinD2, using same TIM1, can't be used at the same time
// pinD4 and pinD5, using same TIM3, can't be used at the same time
// pinD3 and pinD6 are using HRTIM, so the minimum freq must be ~770Hz 
uint32_t myPin  = pinD5;

float dutyCycle = 50.0f;

float freq      = 5000.0f;

mbed::PwmOut* pwm   = NULL;
```

#### 2. Set PWM Frequency and dutycycle

```
void setup()
{
  ....
  
  setPWM(pwm, myPin, freq, dutyCycle);
}  
```

---
---

### Examples: 

 1. [PWM_Multi](examples/PWM_Multi)
 2. [PWM_Single](examples/PWM_Single)
 3. [**multiFileProject**](examples/multiFileProject) **New**

 
---
---

### Example [PWM_Multi](examples/PWM_Multi)

```
#if !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  #error For Portenta_H7 only
#endif

#define _PWM_LOGLEVEL_       1

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "Portenta_H7_PWM.h"

#define LED_ON        LOW
#define LED_OFF       HIGH

// Can't use same TimerIndex again, e.g., the D1 and D2, using TIM1, can't be use concurrently
// That's why D0, D1, D3, D4 and D6 (using TimerIndex 8, 1, HRTIM and 3) are OK together

// Only OK for D0, D1, D2, D4 and D5, PA_0C(D15/A0), PA_1C(D16/A1), 
// D3, D6, D7, D8, D9, D10, D11, D12, D13, D14, D17(PC_2C/A2), D18(PC_3C/3), PC2(D19/A4) LEDG, LEDB not OK
#define pinD0    D0       // PH15 / TIM8_CH3N
#define pinD1    D1       // PK1  / TIM1_CH1, TIM8_CH3
#define pinD2    D2       // PJ11 / TIM1_CH2, TIM8_CH2N
#define pinD3    D3       // PG7  / HRTIM_CHE2
#define pinD4    D4       // PC7  / TIM3_CH2, TIM8_CH2, HRTIM_CHA2
#define pinD5    D5       // PC6  / TIM3_CH1, TIM8_CH1, HRTIM_CHA1, LPTIM3_OUT
#define pinD6    D6       // PA8  / HRTIM_CHB2 (TIM1_CH1, TIM8_BKIN2)

// See https://www.st.com/resource/en/datasheet/stm32h747xi.pdf, Table 7, page 53
// Can't use pins with same TIMx. For example, 
// pinD1 and pinD2, using same TIM1, can't be used at the same time
// pinD4 and pinD5, using same TIM3, can't be used at the same time
// pinD3 and pinD6 are using HRTIM, can't be used at the same time and the minimum freq must be ~770Hz 
uint32_t pins[]       = { pinD0, pinD1, pinD3, pinD5 };

#define NUM_OF_PINS       ( sizeof(pins) / sizeof(uint32_t) )

float dutyCycle[]      = { 50.0f, 50.0f, 50.0f, 50.0f };

float freq[]           = { 1000.0f, 2500.0f, 4000.0f, 5000.0f };

float curDutyCycle[]   = { 50.0f, 50.0f, 50.0f, 50.0f };

float curFreq[]        = { 1000.0f, 2500.0f, 4000.0f, 5000.0f };

mbed::PwmOut* pwm[]    = { NULL, NULL, NULL, NULL };

void startAllPWM()
{
  digitalWrite(LEDG, LED_ON);
  digitalWrite(LEDB, LED_OFF);
  digitalWrite(LEDR, LED_OFF);
  
  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    PWM_LOGERROR7("Freq = ", freq[index], ", \tDutyCycle % = ", dutyCycle[index], ", \tDutyCycle = ", dutyCycle[index] / 100, ", \tPin = ", pins[index]);
    
    // setPWM(mbed::PwmOut* &pwm, pin_size_t pin, float frequency, float dutyCycle)
    setPWM(pwm[index], pins[index], freq[index], dutyCycle[index]);
  }
}

void restoreAllPWM()
{
  digitalWrite(LEDG, LED_ON);
  digitalWrite(LEDB, LED_OFF);
  digitalWrite(LEDR, LED_OFF);
  
  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    curFreq[index]      = freq[index];
    curDutyCycle[index] = dutyCycle[index];
    
    // setPWM(mbed::PwmOut* &pwm, pin_size_t pin, float frequency, float dutyCycle)
    setPWM(pwm[index], pins[index], freq[index], dutyCycle[index]);
  }
}

void changeAllPWM()
{
  digitalWrite(LEDG, LED_OFF);
  digitalWrite(LEDB, LED_ON);
  digitalWrite(LEDR, LED_OFF);
  
  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    curFreq[index]      = freq[index] * 2;
    curDutyCycle[index] = dutyCycle[index] / 2;
    
    // setPWM(mbed::PwmOut* &pwm, pin_size_t pin, float frequency, float dutyCycle)
    setPWM(pwm[index], pins[index], curFreq[index], curDutyCycle[index]);
  }
}

void stopAllPWM()
{
  digitalWrite(LEDG, LED_OFF);
  digitalWrite(LEDB, LED_OFF);
  digitalWrite(LEDR, LED_ON);
  
  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    curFreq[index]      = 1000.0f;
    curDutyCycle[index] = 0.0f;
    
    //stopPWM(mbed::PwmOut* &pwm, pin_size_t pin)
    stopPWM(pwm[index], pins[index]);
  }
}

void printLine()
{
  Serial.println(F("\n=========================================================================================================="));
}

void printPulseWidth()
{
  static uint32_t num = 0;

  if (num++ % 50 == 0)
  {
    printLine();
    
    for (uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
      Serial.print(F("PW (us) ")); Serial.print(index); Serial.print(F("\t"));  
    }

    printLine();
  }
 
  if (num > 1)
  {
    for (uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
      if (pwm[index])
      {
        if ( (pins[index] == pinD3) || (pins[index] == pinD6) )
        {
          // Using HRTIM => fake by calculating PW
          Serial.print( (10000 * curDutyCycle[index]) / curFreq[index] ); Serial.print(F("\t\t")); 
        }
        else
        {
          Serial.print((float) pwm[index]->read_pulsewitdth_us()); Serial.print(F("\t\t")); 
        }
      }
    }

    Serial.println();
  }
}

#define PRINT_INTERVAL      10000L
#define CHANGE_INTERVAL     20000L

void check_status()
{
  static unsigned long checkstatus_timeout  = 0;
  static unsigned long changePWM_timeout    = 0;

  static bool PWM_orig  = true;
  static uint32_t count = 0;

  // Print every PRINT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    printPulseWidth();
    checkstatus_timeout = millis() + PRINT_INTERVAL;
  }

  if ( (millis() > changePWM_timeout) && (millis() > CHANGE_INTERVAL) )
  {
    
    if (PWM_orig)
    {
      if (count++ %2 == 0)
      {
        Serial.println("Stop all PWM");
        stopAllPWM();
      }
      else
      {
        Serial.println("Change all PWM");
        
        changeAllPWM();

        PWM_orig = !PWM_orig;
      }
    }
    else
    {
      Serial.println("Restore all PWM");
      
      restoreAllPWM();

      PWM_orig = !PWM_orig;
    }
      
    changePWM_timeout = millis() + CHANGE_INTERVAL;
  }
}

void setup()
{
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDR, OUTPUT);

  digitalWrite(LEDG, LED_OFF);
  digitalWrite(LEDB, LED_OFF);
  digitalWrite(LEDR, LED_OFF);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    pinMode(pins[index], OUTPUT);
    digitalWrite(pins[index], LOW);
  }

  Serial.begin(115200);
  while (!Serial);

  delay(100);

  Serial.print(F("\nStarting PWM_Multi on ")); Serial.println(BOARD_NAME);
  Serial.println(PORTENTA_H7_PWM_VERSION);

  // Automatically retrieve TIM instance and channel associated to pin
  // This is used to be compatible with all STM32 series automatically.

  startAllPWM();
}

void loop()
{
  check_status();
}
```
---
---

### Debug Terminal Output Samples

### 1. PWM_Single on PORTENTA_H7_M7

The following is the sample terminal output when running example [PWM_Single](examples/PWM_Single) on **Portenta_H7** to demonstrate how to start a single PWM channel, then stop, change, restore the PWM settings on-the-fly.


```
Starting PWM_Single on PORTENTA_H7_M7
Portenta_H7_PWM v2.0.2
[PWM] Freq = 5000.00, DutyCycle % = 50.00, DutyCycle = 0.50, Pin = 5

========
PW (us)
========
100.00		
Stop PWM
0.00		
0.00		
Change PWM
25.00		
25.00		
Restore PWM
100.00		
100.00		
Stop PWM
0.00		
0.00		
Change PWM
25.00		
25.00		
Restore PWM
100.00		
100.00
```

---

### 2. PWM_Multi on PORTENTA_H7_M7

The following is the sample terminal output when running example [**PWM_Multi**](examples/PWM_Multi) on **Portenta_H7** to demonstrate how to start multiple PWM channels, then stop, change, restore the PWM settings on-the-fly.

```

Starting PWM_Multi on PORTENTA_H7_M7
Portenta_H7_PWM v2.0.2
[PWM] Freq = 1000.00, 	DutyCycle % = 50.00, 	DutyCycle = 0.50, 	Pin = 0
[PWM] Freq = 2500.00, 	DutyCycle % = 50.00, 	DutyCycle = 0.50, 	Pin = 1
[PWM] Freq = 4000.00, 	DutyCycle % = 50.00, 	DutyCycle = 0.50, 	Pin = 3
[PWM] Freq = 5000.00, 	DutyCycle % = 50.00, 	DutyCycle = 0.50, 	Pin = 5

==========================================================================================================
PW (us) 0	PW (us) 1	PW (us) 2	PW (us) 3	
==========================================================================================================
500.00		200.00		125.00		100.00		
Stop all PWM
0.00		0.00		0.00		0.00		
0.00		0.00		0.00		0.00		
Change all PWM
125.00		50.00		31.25		25.00		
125.00		50.00		31.25		25.00		
Restore all PWM
500.00		200.00		125.00		100.00		
500.00		200.00		125.00		100.00		
Stop all PWM
0.00		0.00		0.00		0.00		
0.00		0.00		0.00		0.00		
Change all PWM
125.00		50.00		31.25		25.00		
125.00		50.00		31.25		25.00		
Restore all PWM
500.00		200.00		125.00		100.00		
500.00		200.00		125.00		100.00	
```


---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level `_PWM_LOGLEVEL_` from 0 to 4

```cpp
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [Portenta_H7_PWM issues](https://github.com/khoih-prog/Portenta_H7_PWM/issues)

---
---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards

---

## DONE

1. Basic hardware multi-channel PWM for **Portenta_H7**.
2. Add Table of Contents
3. Rewrite the library to fix bug and to permit to start, stop, modify, restore PWM settings on-the-fly
4. Optimize library code by using `reference-passing` instead of `value-passing`
5. Convert to `h-only` style


---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

1. Thanks to [GitChris3004](https://github.com/GitChris3004) for reporting, investigating the bug in [No PWM-Output signals #1](https://github.com/khoih-prog/Portenta_H7_PWM/issues/1), which is fixed and leading to v2.0.0.


<table>
  <tr>
    <td align="center"><a href="https://github.com/GitChris3004"><img src="https://github.com/GitChris3004.png" width="100px;" alt="GitChris3004"/><br /><sub><b>GitChris3004</b></sub></a><br /></td>
  </tr>
</table>

---

## Contributing

If you want to contribute to this project:

- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/Portenta_H7_PWM/blob/master/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


