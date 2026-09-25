/*------------------------------------------------------------
Project : TIME-STAMPED SENSOR DATA LOGGER
File    : Mini_Main.c
MCU     : LPC2129 ARM7 (LPC21xx series)
Purpose : Initialize all required peripherals and run the 
          time-stamped sensor data logging system.
Modules : UART0, RTC, ADC (LM35), LCD, Keypad, GPIO LED
------------------------------------------------------------
Objective:
A Time-Stamped Sensor Data Logger is a system that continuously or periodically
records sensor readings along with accurate date and time information into a log
file for later analysis. It ensures traceability, fault analysis, and historical data
tracking, which are essential in industrial and research applications.
------------------------------------------------------------*/

// MCU register definition header
#include<LPC214x.h>

// Master header containing all driver prototypes and pin definitions
#include "Mini_headers.h"


//------------------------------------------------------------
// Function: main
// Purpose : Entry point of firmware – Initialize peripherals 
//           and start sensor logging system
//------------------------------------------------------------
int  main()
{
	// Initialize UART0 for serial terminal debug/log output
	InitUART();	
	
	// Initialize Real Time Clock (RTC) for 1Hz time base
	RTC_Init();	
	
	// Initialize ADC Channel 0 for LM35 temperature sensor
	Init_ADC(CH1);
	
	// Initialize 16x2 LCD in 8-bit mode.
	InitLCD();
	
	// Initialize 4x4 keypad
	KeyPdInit();
	
	// Initialize system logic for time-stamped sensor data logging
	System_Init();
}

