/*------------------------------------------------------------
File: Mini_Defines.h
Purpose: Pin mapping and function prototypes for LPC2129 ARM7
embedded mini-project involving:
- UART0 communication
- Real Time Clock (RTC)
- ADC (LM35 temperature sensor read)
- 16x2 LCD in 8-bit mode
- Status LED indication
------------------------------------------------------------*/

// Header with LPC21xx register definitions
#include<LPC214x.h>

// Fixed-width datatype header (u32, s32, f32, u8, s8 etc.)
#include "types.h"

//---------------------------------------------------------
// LCD GPIO Pin Mapping (16x2 LCD in 8-bit mode)
//---------------------------------------------------------
#define LCD_DAT 0xff  // LCD data lines
#define RS 10 			  // Register Select  P0.10
#define RW 11				  // Read/Write Select  P0.11
#define EN 12				  // Enable pulse  P0.12

//-------------------------------------------------------------------
// Configuration functions for selecting GPIO and etc.. Pin Mapping
//-------------------------------------------------------------------
#define FN1 0
#define FN2 1
#define FN3 2
#define FN4 3

/*------------------------------------------------------------
Define 4×4 keypad row/column bit positions for GPIO scanning.
------------------------------------------------------------*/
#define R0 24  	// Keypad Row0 scan control bit position (mapped to P1.24)
#define R1 25		// Keypad Row1 scan control bit position (mapped to P1.25)
#define R2 26		// Keypad Row2 scan control bit position (mapped to P1.26)
#define R3 27		// Keypad Row3 scan control bit position (mapped to P1.27)

#define C0 20		// Keypad Column0 read bit position (mapped to P1.20)
#define C1 21		// Keypad Column1 read bit position (mapped to P1.21)
#define C2 22		// Keypad Column2 read bit position (mapped to P1.22)
#define C3 23		// Keypad Column3 read bit position (mapped to P1.23)


//---------------------------------------------------------
// GPIO Pin Mapping for status LED and Switch input
//---------------------------------------------------------
#define LED 16 // Status LED output  P1.16
#define SW 17	 // External switch input  P1.17

//------------------------------------------------------------
// UART0 Function Prototypes
//------------------------------------------------------------
void InitUART(void);			 // Initialize UART0 on P0.0(TX) and P0.1(RX)
void UARTTxChar(s8 ch);		 // Transmit a single character via UART0
void UARTTxStr(s8 *ptr);	 // Transmit a null-terminated string via UART0
void UARTTxU32(u32 num);	 // Transmit 32-bit unsigned integer via UART0
void UARTTxF32(f32 fnum);	 // Transmit float value via UART0 (6 decimal places)

//------------------------------------------------------------
// RTC Function Prototypes
//------------------------------------------------------------
void RTC_Init(void);	 																		  // Reset and enable RTC with 1Hz prescaler
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second);		// Read current RTC time
void DisplayRTCTime(u32 hour, u32 minute, u32 second);			// Display time on LCD in HH:MM:SS
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year);			// Read current RTC date
void DisplayRTCDate(u32 date, u32 month, u32 year);					// Display date on LCD in DD/MM/YYYY
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second);			// Write new Hour, Minute and Second
void SetRTCDateInfo(u32 date, u32 month, u32 year);					// Write new Date, Month and Year
void GetRTCDay(s32 *day);																	  // Read Day of Week (0=SUN … 6=SAT)
void SetRTCDay(u32 day);																		// Write new Day of Week into RTC
void DisplayRTCDay(u32 dow);																// Display 3-character day string on LCD
u8 ValidateRTCDate(u32 d, u32 m, u32 y);										// Checking for valid Date/Month/Year

//------------------------------------------------------------
// ADC + LM35 Temperature Sensor Function Prototypes
//------------------------------------------------------------
void Init_ADC(u32 chNo);															// Initialize ADC channel on AIN pins
void Read_ADC(u32 chNo, f32 *eAR, u32 *adcDVal);			// Start ADC, wait DONE, read 10-bit
f32 Read_LM35_NP(u8 tType);														// Differential LM35 temperature read
void DisplayTemp(u32 temp);													 	// Display 2-digit temperature on LCD

//------------------------------------------------------------
// LCD Driver Function Prototypes
//------------------------------------------------------------
void InitLCD(void);			// Initialize LCD GPIO and 8-bit mode
void CmdLCD(u8 cmd);		// Send LCD command (RS=0)
void CharLCD(u8 dat);		// Send LCD data character (RS=1)
void DispLCD(u8 val);		// Write byte to LCD data lines, generate EN pulse
void StrLCD(u8 *ptr);		// Print string on LCD
void IntLCD(s32 num);		// Print signed integer on LCD

//------------------------------------------------------------
// Keypad Function Prototype
//------------------------------------------------------------
void KeyPdInit(void);
u8 ColStat(void);					// Detect if any keypad column pin is LOW (pressed)
u8 KeyVal(void);					// Scan keypad rows and decode active column to return mapped key from lookup table
int GetKeyPress(void);		// Wait for key press ? read key ? wait for release

//------------------------------------------------------------
// Main System Initialization Function Prototype
//------------------------------------------------------------
void System_Init(void);

//------------------------------------------------------------
// Software delay prototypes
// Used for blocking delays when Timer hardware is not required
//------------------------------------------------------------
void delay_us(unsigned int);				// Delay in microseconds
void delay_ms(unsigned int);				// Delay in milliseconds
void delay_s(unsigned int);					// Delay in seconds

//------------------------------------------------------------
// Purpose  : Configure LPC2129 pin for peripheral/GPIO function
//------------------------------------------------------------

void CfgPinFunc(int,int,int);






