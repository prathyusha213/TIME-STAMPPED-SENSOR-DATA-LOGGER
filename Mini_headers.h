/*------------------------------------------------------------
File: Mini_headers.h
Purpose: Centralized header include list for LPC2129 ARM7
Embedded mini-project involving:
- ADC for LM35 temperature sensor
- UART0 communication
- RTC time/date/day handling
- 16x2 LCD display driver
- 4x4 Keypad interface
- GPIO pin connections
- Software delay utilities
------------------------------------------------------------*/


//------------------------------------------------------------
// Core Peripheral Drivers
//------------------------------------------------------------
#include "types.h"					// Standard datatype definitions

//------------------------------------------------------------
// ADC + Temperature Sensor
//------------------------------------------------------------
#include "adc_defines_mini.h"			// ADC register bit masks and macros
#include "lm35_mini.h"						// LM35 temperature conversion logic

//------------------------------------------------------------
// RTC (Real Time Clock)
//------------------------------------------------------------
#include "rtc_defines_mini.h"			// RTC register bit masks and values

//------------------------------------------------------------
// Main project pin mappings, macros & function prototypes
//------------------------------------------------------------
#include "defines.h"							//pin mappings, macros & function prototypes

//------------------------------------------------------------
// Mini-project macro and constant definitions
//------------------------------------------------------------
#include "Mini_Defines.h"			//Project Related Defines


