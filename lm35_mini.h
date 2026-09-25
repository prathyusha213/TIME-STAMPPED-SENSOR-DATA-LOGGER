/*-----------------------------------------------------------
File    : lm35_mini.h
Purpose : Declare LM35 temperature sensor read APIs using ADC.
          Supports normal read and differential read modes.
------------------------------------------------------------*/

// Include user-defined data type aliases (u8, u32, f32, etc.)
#include "types.h"

f32 Read_LM35(u8 tType);						// Read LM35 temperature (single-ended ADC) and return in °C or °F
f32 Read_LM35_NP(u8 tType);					// Read LM35 temperature using differential ADC (CH0–CH1) and return in °C or °F
