/*------------------------------------------------------------
File    : rtc_defines_mini.h
Purpose : Define clock settings and RTC register bit masks
          for RTC initialization on LPC2129 ARM7 MCU.
------------------------------------------------------------*/
#ifndef RTC_DEFINES_H							// Start of include guard to prevent multiple inclusion		
#define RTC_DEFINES_H							// Define guard macro to block re-including this header	
	
// System clock and peripheral clock Macros
#define FOSC 12000000							// External crystal oscillator frequency = 12 MHz
#define CCLK (FOSC*5)							// CPU clock derived from FOSC using PLL multiplier (5x ? 60 MHz)
#define PCLK (CCLK/4) 						// Peripheral clock runs at 1/4 of CPU clock (15 MHz)

// RTC Macros
#define PREINT_VAL ((PCLK/32768)-1)			// RTC prescaler integer part for 1Hz clock from 32.768kHz base	
#define PREFRAC_VAL (PCLK-(PREINT_VAL+1)*32768)			// RTC fractional prescaler part for fine adjustment

//CCR register bits 
#define RTC_ENABLE  (1<<0)					// CCR bit0 ? Enable RTC counter
#define RTC_RESET   (1<<1)					// CCR bit1 ? Reset RTC counter and prescaler
#define RTC_CLKSRC  (1<<4)					// CCR bit4 ? Select external 32.768kHz clock source	

#endif															 // End of include guard
