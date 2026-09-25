/*------------------------------------------------------------
File    : mini_system_init.c
MCU     : LPC2129 ARM7 (LPC21xx Series)
Purpose : Initialize peripherals and run continuous time-stamped
          temperature logging with keypad-based RTC editing and
          temperature setpoint update.
Peripherals:
  - UART0  ? Serial terminal debug/log output
  - RTC    ? Time/Date/Day tracking (1Hz tick)
  - ADC    ? LM35 differential temperature read (AIN0, AIN1)
  - LCD    ? 16x2 display in 8-bit mode (P0.0–P0.7, RS, RW, EN)
  - Keypad ? 4x4 matrix keypad (P1.16–P1.23)
  - LED    ? Status indicator on P0.16
------------------------------------------------------------ */
					
//Header with LPC21xx register definitions
#include<LPC214x.h>

// Header containing project-specific macros, typedefs and prototypes
#include "Mini_headers.h"

//---------------------------------------------------------
// Global RTC storage variables
//---------------------------------------------------------
s32 hour,min,sec,date,month,year,day;
extern char week[][4];

//---------------------------------------------------------
// Global KEYPAD variables
//---------------------------------------------------------
extern u8 KEY[4][4];

//------------------------------------------------------------
// Function: System_Init
// Purpose : Initialize system and run sensor logging + edit UI
//------------------------------------------------------------
void System_Init(void)
{
		float currentTemp;			// Stores the LM35 differential voltage result
		int setpoint=46;				// Default temperature threshold setpoint value
		int cnt = 0;						// Counter flag to limit boot print repetition
		int u=0;								// Exit flag to limit boot print repetition
		int PrintOnce = 0;			// Print flag to limit boot print repetition
	
		// Configure status LED pin as output - P0.16
    IODIR0 |= (1 << LED);
    IOCLR0  = (1 << LED); // LED OFF initially
	
		//Set the initial time (hours, minutes, seconds)
		SetRTCTimeInfo(12,14,00);
		SetRTCDateInfo(30,10,2025);
		SetRTCDay(4);

		//Print initial temperature test once
		if(PrintOnce == 0)
		{
				UARTTxStr("LM35 TEST: \n\r");						// UART initial test message to verify LM35 sensor interface
				UARTTxStr("Temperature : ");						// UART message label for temperature print
				UARTTxF32(Read_LM35_NP('C'));						// Print LM35 temperature reading in °C without clearing UART screen
				UARTTxStr(" deg C \n\r");								// UART message for degree + unit
		}
		
		// Main sensor logging loop
		while(1)
		{
					int pres=0,prev=1;										// Flags for switch press edge detection
					
					INPUT:
					GetRTCTimeInfo(&hour,&min,&sec);			// Read RTC time registers into global variables via pointers
					GetRTCDateInfo(&date,&month,&year);		// Read RTC date registers into global variables	
					GetRTCDay(&day);											// Read RTC Day of week register into variable
					
					DisplayRTCTime(hour,min,sec);					// Display time on LCD in HH:MM:SS format
					DisplayRTCDate(date,month,year);			// Display date on LCD in DD/MM/YYYY format
					DisplayRTCDay(day);										// Display current day string (week[day]) on LCD
					
					currentTemp = Read_LM35_NP('C'); 			// Read LM35 temperature using differential ADC read (CH0–CH1)
					
					DisplayTemp((u32)currentTemp);				// Display temperature on LCD as 2-digit integer
					CharLCD(0xDF);
					//DisplayTemp(setpoint);												// Append degree symbol (0xDF) to LCD
					CharLCD('C');													// Print character 'C' for Celsius on LCD

					// If temperature is safe (below setpoint)
					if(currentTemp < setpoint)
					{
						IODIR1|=(1<<LED);										// Configure LED pin direction register as output
						IOCLR1 = 1<<LED;										// Turn OFF LED (safe temp indication)
					
						// Print once at boot for first safe reading
						if(cnt == 0)
						{
							cnt++;														// Prevent repeat boot print
							
							UARTTxStr("Temp: "); 							// UART label for logged temperature
							UARTTxF32(currentTemp); 					// UART print current temperature in float format
							UARTTxStr("\xF8"); 								// Send ASCII degree symbol
							UARTTxStr("C @ ");								// UART label for timestamp

							UARTTxStr(" ");										// Print space separator
							
							if(hour<10) 											// Print hour with zero padding
								UARTTxChar('0'); 								
							UARTTxU32(hour); 									
							UARTTxChar(':');									// Print ':' separator
							if(min<10)  											// Print minutes with zero padding
								UARTTxChar('0');
							UARTTxU32(min);  
							UARTTxChar(':');									// Print ':' separator
							if(sec<10)  											// Print seconds with zero padding
								UARTTxChar('0'); 
							UARTTxU32(sec);
							
							UARTTxStr(" ");										// Print space separator
							
							if(date < 10) 										// Print date with padding
								UARTTxStr("0"); 
							UARTTxU32(date); 
							UARTTxStr("/"); 									// Print '/' separator
							if(month < 10) 										// Print month with padding
								UARTTxStr("0"); 
							UARTTxU32(month); 									
							UARTTxStr("/");										// Print '/' separator
							UARTTxU32(year); 									// Print year
							UARTTxStr("\n\r");								// New line
						}
					}				

					// Print every minute at sec==0
					if((currentTemp < setpoint) && sec==0) 
					{ 	
							UARTTxStr("Temp: "); 							// UART label for logged temperature
							UARTTxF32(currentTemp); 					// UART print current temperature in float format
							UARTTxStr("\xF8"); 								// Send ASCII degree symbol
							UARTTxStr("C @ ");								// UART label for timestamp

							UARTTxStr(" ");										// Print space separator
							
							if(hour<10) 											// Print hour with zero padding
								UARTTxChar('0'); 								
							UARTTxU32(hour); 									
							UARTTxChar(':');									// Print ':' separator
							if(min<10)  											// Print minutes with zero padding
								UARTTxChar('0');
							UARTTxU32(min);  
							UARTTxChar(':');									// Print ':' separator
							if(sec<10)  											// Print seconds with zero padding
								UARTTxChar('0'); 
							UARTTxU32(sec);
							
							UARTTxStr(" ");										// Print space separator
							
							if(date < 10) 										// Print date with padding
								UARTTxStr("0"); 
							UARTTxU32(date); 
							UARTTxStr("/"); 									// Print '/' separator
							if(month < 10) 										// Print month with padding
								UARTTxStr("0"); 
							UARTTxU32(month); 									
							UARTTxStr("/");										// Print '/' separator
							UARTTxU32(year); 									// Print year
							UARTTxStr("\n\r");								// New line
						}
					
						// If temperature crosses setpoint ? alert case
						else if(currentTemp > setpoint)
						{ 
							cnt=0;														// Reset counter so alert prints can repeat again
							{
								UARTTxStr("[ALERT!] "); 				// Print Alert message when current temp is above set point
								UARTTxStr("Temp: "); 						// UART label for logged temperature
								UARTTxF32(currentTemp); 				// UART print current temperature in float format
								UARTTxStr("\xF8"); 							// Send ASCII degree symbol
								UARTTxStr("C @ ");							// UART label for timestamp

								UARTTxStr(" ");									// Print space separator
							
								if(hour<10) 										// Print hour with zero padding
									UARTTxChar('0'); 								
								UARTTxU32(hour); 									
								UARTTxChar(':');								// Print ':' separator
								if(min<10)  										// Print minutes with zero padding
									UARTTxChar('0');
								UARTTxU32(min);  
								UARTTxChar(':');								// Print ':' separator
								if(sec<10)  										// Print seconds with zero padding
									UARTTxChar('0'); 
								UARTTxU32(sec);
								
								UARTTxStr(" ");									// Print space separator
								
								if(date < 10) 									// Print date with padding
									UARTTxStr("0"); 
								UARTTxU32(date); 
								UARTTxStr("/"); 								// Print '/' separator
								if(month < 10) 									// Print month with padding
									UARTTxStr("0"); 
								UARTTxU32(month); 									
								UARTTxStr("/");									// Print '/' separator
								UARTTxU32(year); 								// Print year
								UARTTxStr(" - OVER TEMP"); 			// Print Overtemp message when current temp is above set point
								UARTTxStr("\n\r");							// New line
								
								IODIR1|=(1<<LED);								// Configuration of LED as OUTPUT
								IOCLR1 = 1<<LED;								// LED OFF initially
								IOSET1 = 1<<LED; 							  // LED ON at P0.16 
							}
						}
							
						// Configure SW pin as input (P0.17)
						PINSEL2 &= ~(1<<SW);
						IODIR1 &= ~(1<<SW);
			
						pres = (((IOPIN1 >> SW)&1)==0);			// Read switch press state (active LOW)
					
					// If switch is pressed and previous state was not pressed
					if(pres && prev)
					{ 
						delay_ms(50);												// debounce delay
						
						// Confirm switch is still pressed after debounce
						if(pres && prev)
						{
							// Show edit mode menu on LCD
							IN1:
							CmdLCD(0x01);
							CmdLCD(0x80);
							StrLCD("1.EDIT RTC INFO");
							CmdLCD(0xC0);
							StrLCD("2.E.SET");
							CmdLCD(0xCA);
							StrLCD("3.EXT");
							
							if(u==0)
							{
							// UART message for debug edit mode entry
							UARTTxStr("*** EDIT MODE ACTIVATED ***\n\r");
							u++;
							}
						
							// Inner loop for keypad option handling
							while(1)
							{
								int opt,field,press=0;					// option and field selection variables
								delay_ms(10); 									// Wait for keypad key press
								
								opt = GetKeyPress();						// Read debounced key press from keypad
								
								
								//------------------------------------------------------------
								// If option == 1 - RTC editing menu
								//------------------------------------------------------------	
								if(opt ==1)											
								{
									IN:
									CmdLCD(0x01);
									CmdLCD(0x80);
									StrLCD("1.H 2.MI 3.S 4.D");
									CmdLCD(0xC0);
									StrLCD("5.M 6.Y 7.DY 8.E");
									
									if(press == 0)								 // Print activation message once
									{
										UARTTxStr("*** Time Editing Mode Activated ***\n\r");
										press++;
									}
									
									field = GetKeyPress();				 // Read which RTC field to edit
									
									// If invalid field then restart
									if(field == 0 || field == 10 ||field == 11 ||field == 12 ||field == 13 ||field == 14 ||field == 15) 
										goto IN;
									
									//------------------------------------------------------------
									// If field == 1 - HOUR editing mode
									//------------------------------------------------------------	
									if(field == 1)
									{
											int newHour;												// Variable to hold newly entered hour value from keypad
										
											CmdLCD(0x01);												// Clear the LCD screen
											CmdLCD(0x80);												// Move LCD cursor to 1st line, 1st character position
											StrLCD("SET HOUR:");								// Display prompt message asking user to enter hour

											// Infinite loop for digit entry + validation + save confirmation
											while (1)
											{
													s8 h1 = -1, h2 = -1;						// Variables to hold first and second digit of hour
													int key;												// Stores decoded keypad key value

													//----------------------------------------------------
													// Display current hour on LCD before user edits
													//----------------------------------------------------
													CmdLCD(0x8A);										// Move cursor to hour display position
													StrLCD("  ");										// Clear the digit area (2 spaces)
													CmdLCD(0x8A);										// Reset cursor to same position again
												
													// Display hour with 2-digit padding if needed
													if(hour>9)											
													{
														IntLCD(hour);									// Show hour directly if 2-digit
													}
													else
													{
														IntLCD(0);										// Print leading zero
														IntLCD(hour);									// Print single digit hour
													}
													CmdLCD(0x8A);										// Keep cursor at hour position
													CmdLCD(0x0E);										// Enable cursor underline ON (for user typing visibility)
													

												  //----------------------------------------------------
													// Read first digit from keypad
													//----------------------------------------------------
													while (1)
													{
															key = KeyVal();					  	// Get decoded key from keypad matrix
														
														  if (key == 13) 							// If CANCEL key pressed before typing
															{
																	CmdLCD(0x0C);						// Turn OFF cursor
																	CmdLCD(0x01);						// Clear LCD
																	goto IN; 								// Jump to main menu without saving
															}
															if (key >= 0 && key <= 9)		// If key is numeric (0-9)
															{
																	h1 = key;								// Store first digit
																	CmdLCD(0x0E);						// Keep cursor ON
																	CharLCD(h1 + '0');			// Display digit as ASCII
																	delay_ms(200);					// Small delay for user readability
																	break;									// Exit after 1st digit
															}
													}

													//----------------------------------------------------
													// Read second digit from keypad
													//----------------------------------------------------
													while (1)													
													{
															key = KeyVal();								// Get decoded key from keypad matrix
															if (key >= 0 && key <= 9)			
															{
																	h2 = key;									// Store second digit
																	CmdLCD(0x0E);							// Keep cursor ON		
																	CharLCD(h2 + '0');				// Display digit as ASCII
																	delay_ms(200);						// Small delay for user readability
																	break;										// Exit after 2nd digit
															}
													}

													//----------------------------------------------------
													// Convert two digits into integer hour format
													//----------------------------------------------------
													newHour = (h1 * 10) + h2;

													//----------------------------------------------------
													// Validate hour range 00-23
													//----------------------------------------------------
													if (newHour < 0 || newHour > 23)
													{
															CmdLCD(0xC0);									// Move cursor to 2nd line
															StrLCD("INVALID! 00-23");			// Show error message
															delay_ms(800);								// Hold message for visibility
															CmdLCD(0x01);									// Clear LCD for retry
															CmdLCD(0x80);									// Reset cursor to first line
															StrLCD("SET HOUR:");					// Show prompt again
															continue;											// Restart loop for correct entry
													}

													//----------------------------------------------------
													// Ask user to SAVE or EDIT again
													//----------------------------------------------------
													CmdLCD(0xC0);											// Move cursor to 2nd line
													StrLCD("SAVE      EDIT");					// Display save/edit choice menu
													delay_ms(300);										// Small hold before decision

													//----------------------------------------------------
													// Wait for SAVE (13) or EDIT (14)
													//----------------------------------------------------
													while (1)
													{
															int K = KeyVal();							// Read user choice from keypad

															// If EDIT key pressed ? retry entry
															if (K == 14)
															{
																	CmdLCD(0x01);							// Clear LCD
																	CmdLCD(0x80);							// Move cursor to 1st line
																	StrLCD("SET HOUR:");			// Prompt again
																	break;  									// Break inner loop to retry digit entry
															}

															// If SAVE key pressed ? write to RTC and exit
															if (K == 13)
															{
																	hour = newHour;						// Update global hour variable
																	SetRTCTimeInfo(hour, min, sec);	// Save hour, keep min/sec same
																	CmdLCD(0x0C);							// Turn OFF cursor
																	CmdLCD(0x01);							// Clear LCD	
																	goto IN;									// Return to main menu after saving	
															}

															delay_ms(150);								// Polling delay while waiting for valid choice
													}
											}
									}
									
									//------------------------------------------------------------
									// If field == 2 - MINUTE editing mode
									//------------------------------------------------------------					
									else if(field == 2)
									{
											int newMin;														// Variable to hold new minute entered
											CmdLCD(0x01);													// Clear LCD
											CmdLCD(0x80);													// Cursor to 1st line
											StrLCD("SET MIN:");										// Minute edit prompt

											// Infinite loop for 2-digit minute entry + validation + save/edit
											while (1)
											{
													s8 m1 = -1, m2 = -1;						 // Store 1st and 2nd minute digits
													int key;												 // Stores keypad decoded key

													// Show current minute on LCD before editing
													CmdLCD(0x8A);										// Cursor to minute display position
													StrLCD("  ");										// Clear digit area
													CmdLCD(0x8A);	

													// 2-digit padding display
													if(min>9)
													{
														IntLCD(min);
													}
													else
													{
														IntLCD(0);
														IntLCD(min);
													}
													CmdLCD(0x8A);
													CmdLCD(0x0E);										// Cursor ON
													
												
													// ---- Read 1st digit ----
													while (1)
													{
															key = KeyVal();							// Decode key
														
															if (key == 13) 							// CANCEL pressed BEFORE entering
															{
																	CmdLCD(0x0C);
																	CmdLCD(0x01);
																	goto IN; 								// Jump to main menu without saving
															}
															if (key >= 0 && key <= 9)
															{
																	m1 = key;
																	CmdLCD(0x0E);
																	CharLCD(m1 + '0');
																	delay_ms(200);
																	break;
															}
													}

													// Read 2nd digit from keypad
													while (1)
													{
															key = KeyVal();
															if (key >= 0 && key <= 9)
															{
																	m2 = key;
																	CmdLCD(0x0E);
																	CharLCD(m2 + '0');
																	delay_ms(200);
																	break;
															}
													}
													newMin = (m1 * 10) + m2;					 // Combine digits

													// Validate minute range 00-59
													if (newMin < 0 || newMin > 59)
													{
															CmdLCD(0xC0);
															StrLCD("INVALID! 00-59");
															delay_ms(800);
															CmdLCD(0x01);
															CmdLCD(0x80);
															StrLCD("SET MIN:");
															continue;
													}

													//----------------------------------------------------
													// Ask user to SAVE or EDIT again
													//----------------------------------------------------

													CmdLCD(0xC0);
													StrLCD("SAVE      EDIT");
													delay_ms(300);

													//----------------------------------------------------
													// Wait for SAVE (13) or EDIT (14)
													//----------------------------------------------------
													while (1)
													{
															int K = KeyVal();

															if (K == 14) 									// EDIT - retry
															{
																	CmdLCD(0x01);
																	CmdLCD(0x80);
																	StrLCD("SET MIN:");
																	break; 										// restart digit entry
															}

															if (K == 13) 									// SAVE - write to RTC
															{
																	min = newMin;
																	SetRTCTimeInfo(hour, min, sec);
																	CmdLCD(0x0C);
																	CmdLCD(0x01);
																	goto IN;
															}


															delay_ms(150);
													}
											}
									}
									
									//------------------------------------------------------------
									// If field == 3 ? SECOND editing mode
									//------------------------------------------------------------
									else if(field == 3)
									{
											int newSec;								// Variable to hold new second entered
										
											CmdLCD(0x01);
											CmdLCD(0x80);
											StrLCD("SET SEC:");				// Prompt for seconds

											while (1)
											{
													s8 s1 = -1, s2 = -1;	// Variables to hold second digits
													int key;							// Keypad decoded key value

													// Show current second on LCD before editing
													CmdLCD(0x8A);
													StrLCD("  ");
													CmdLCD(0x8A);
												
													// 2-digit padding for seconds
													if(sec>9)
													{
														IntLCD(sec);
													}
													else
													{
														IntLCD(0);
														IntLCD(sec);
													}
													CmdLCD(0x8A);
													CmdLCD(0x0E);
													
													// ---- Read 1st digit ----
													while (1)
													{
															key = KeyVal();
														
															if (key == 13) 						 // CANCEL pressed BEFORE entering
															{
																	CmdLCD(0x0C);
																	CmdLCD(0x01);
																	goto IN;							 // Jump to main menu without saving
															}
															if (key >= 0 && key <= 9)
															{
																	s1 = key;
																	CharLCD(s1 + '0');
																  CmdLCD(0x0E);
																	delay_ms(300);
																	break;
															}
													}

													// ---- Read 2nd digit ----
													while (1)
													{
															key = KeyVal();
														
															if (key >= 0 && key <= 9)
															{
																	s2 = key;
																	CmdLCD(0x0E);
																	CharLCD(s2 + '0');
																	delay_ms(200);
																	break;
															}
													}
													newSec = (s1 * 10) + s2;				// Combine digits

													// Validate seconds range 00-59
													if (newSec < 0 || newSec > 59)
													{
															CmdLCD(0xC0);
															StrLCD("INVALID! 00-59");
															delay_ms(800);
															CmdLCD(0x01);
															CmdLCD(0x80);
															StrLCD("SET SEC:");
															continue;
													}

													//----------------------------------------------------
													// Ask user to SAVE or EDIT again
													//----------------------------------------------------
													CmdLCD(0xC0);
													StrLCD("SAVE      EDIT");
													delay_ms(300);

													//----------------------------------------------------
													// Wait for SAVE (13) or EDIT (14)
													//----------------------------------------------------
													while (1)
													{
															int K = KeyVal();

															if (K == 14) 													// EDIT - retry
															{
																	CmdLCD(0x01);
																	CmdLCD(0x80);
																	StrLCD("SET SEC:");
																	break; 														// restart digit entry
															}

															if (K == 13) 													// SAVE - write to RTC
																	sec = newSec;
																	SetRTCTimeInfo(hour, min, sec);
																	CmdLCD(0x0C);
																	CmdLCD(0x01);
																	goto IN;
															}


															delay_ms(150);
													}
											}
						
									//------------------------------------------------------------
									// If field == 4 ? DATE (Day of Month) editing mode
									//------------------------------------------------------------
									else if(field == 4)
									{
											int newdate;							// Stores new date entered
										
											DE:												// DE ? Date entry retry point
											CmdLCD(0x01);
											CmdLCD(0x80);
											StrLCD("SET DATE:");			// Prompt for date

											while (1)
											{
													s8 d1 = -1, d2 = -1;
													int key;
											
													// Display current date before editing
													CmdLCD(0x8A);
													StrLCD("  ");
													CmdLCD(0x8A);
													if(date > 9)
													{
															IntLCD(date);
													}
													else 
													{
															IntLCD(0);
															IntLCD(date);
													}
													CmdLCD(0x8A);
													CmdLCD(0x0E);

													// ---- Read 1st digit ----
													while (1)
													{
															key = KeyVal();
														
															if (key == 13) 							// CANCEL pressed BEFORE entering
															{
																	CmdLCD(0x0C);
																	CmdLCD(0x01);
																	goto IN; 								// Jump to main menu without saving
															}
															if (key >= 0 && key <= 9)
															{
																	d1 = key;
																	CharLCD(d1 + '0');
																	delay_ms(300);
																	break;
															}
													}

													// ---- Read 2nd digit ----
													while (1)
													{
															key = KeyVal();
															if (key >= 0 && key <= 9)
															{
																	d2 = key;
																	CharLCD(d2 + '0');
																	delay_ms(200);
																	break;
															}
													}

													newdate = (d1 * 10) + d2;

													//----------------------------------------------------
													// Ask user to SAVE or EDIT again
													//----------------------------------------------------
													CmdLCD(0xC0);
													StrLCD("SAVE      EDIT");

													//----------------------------------------------------
													// Wait for SAVE (13) or EDIT (14)
													//----------------------------------------------------
													while (1)
													{
															key = KeyVal();

															if (key == 14) 												// EDIT - retry
															{
																	CmdLCD(0x01);
																	CmdLCD(0x80);
																	StrLCD("SET DATE:");
																	break;
															}

															if (key == 13) 												// SAVE - validate & store
															{
																	
																	int maxDate;

																	if(month == 2)
																	{
																			if(year % 4 == 0) maxDate = 29;
																			else maxDate = 28;
																	}
																	else if(month == 4 || month == 6 || month == 9 || month == 11)
																			maxDate = 30;
																	else
																			maxDate = 31;

																	if(newdate < 1 || newdate > maxDate)

																	// Validate new date using RTC date validator
																	if(!ValidateRTCDate(newdate, month, year))
																	{
																			CmdLCD(0x01);
																			CmdLCD(0x80);
																			StrLCD("INVALID DATE!");
																			delay_ms(1000);
																			goto DE;
																	}

																	// Save to RTC
																	date = newdate;
																	SetRTCDateInfo(date, month, year);
																	CmdLCD(0x0C);
																	CmdLCD(0x01);
																	goto IN;
															}

															delay_ms(150);
													}
											}
									}

									//------------------------------------------------------------
									// If field == 5 - MONTH editing mode
									//------------------------------------------------------------
									else if(field == 5)
									{
											int newMon;
										
											M1:												 // M1 - Month entry retry point
											CmdLCD(0x01);
											CmdLCD(0x80);
											StrLCD("SET MON:");

											while (1)
											{
													s8 M1 = -1, M2 = -1;
													int key;

													 // Display current month before editing
													CmdLCD(0x8A);
													StrLCD("  ");
													CmdLCD(0x8A);
												
													// Print current month padded
													if(month>9)
													{
														IntLCD(month);
													}
													else
													{
														IntLCD(0);
														IntLCD(month);
													}
													CmdLCD(0x8A);
													CmdLCD(0x0E);

													// ---- Read 1st digit ----
													while (1)
													{
															key = KeyVal();
														
															if (key == 13) 				// CANCEL pressed BEFORE entering
															{
																	CmdLCD(0x0C);
																	CmdLCD(0x01);
																	goto IN; 					// Jump to main menu without saving
															}
															if (key >= 0 && key <= 9)
															{
																	M1 = key;
																	CmdLCD(0x0E);
																	CharLCD(M1 + '0');
																	delay_ms(200);
																	break;
															}
													}

													// ---- Read 2nd digit ----
													while (1)
													{
															key = KeyVal();
															if (key >= 0 && key <= 9)
															{
																	M2 = key;
																	CmdLCD(0x0E);
																	CharLCD(M2 + '0');
																	delay_ms(200);
																	break;
															}
													}

													newMon = (M1 * 10) + M2;

													// Validate month range 1–12
													if (newMon < 1 || newMon > 12)
													{
															CmdLCD(0xC0);
															StrLCD("INVALID! 1-12");
															delay_ms(800);
															CmdLCD(0x01);
															CmdLCD(0x80);
															StrLCD("SET MON:");
															continue;
													}

													//----------------------------------------------------
													// Ask user to SAVE or EDIT again
													//----------------------------------------------------
													CmdLCD(0xC0);
													StrLCD("SAVE      EDIT");
													delay_ms(300);

													//----------------------------------------------------
													// Wait for SAVE (13) or EDIT (14)
													//----------------------------------------------------
													while (1)
													{
															int K = KeyVal();

															if (K == 14) 													// EDIT - retry
															{
																	CmdLCD(0x01);
																	CmdLCD(0x80);
																	StrLCD("SET MON:");
																	break; // restart digit entry
															}

															if (K == 13)												 // SAVE key pressed - update RTC and exit
															{
																
																if(!ValidateRTCDate(date, newMon, year)) 
																{
																		CmdLCD(0x01);
																		CmdLCD(0x80);
																		StrLCD("INVALID MONTH!");
																		CmdLCD(0xC0);
																		StrLCD("Not Saved");
																		UARTTxStr("[WARN] Invalid Date/Month/Year combination!\n\r");
																		delay_ms(1200);
																		CmdLCD(0x01);
																		goto M1;
																}
																
																month = newMon;
																SetRTCDateInfo(date, month, year);
																CmdLCD(0x0C);
																CmdLCD(0x01);
																goto IN;
															}

															delay_ms(150);
													}
											}
									}
								
									
									//------------------------------------------------------------
									// If field == 6 - YEAR editing mode
									//------------------------------------------------------------
									else if(field == 6)
									{
											int newYear;
										
											Y1:														 //Y1 ? Year entry retry point
											CmdLCD(0x01);
											CmdLCD(0x80);
											StrLCD("SET YEAR:");

											while (1)
											{
													s8 y1 = -1, y2 = -1, y3 = -1, y4 = -1;
													int key;

													 // Display current year before editing
													CmdLCD(0x8A);
													StrLCD("  ");
													CmdLCD(0x8A);
												
													// Print current year padded
													if(year>9)
													{
														IntLCD(year);
													}
													else
													{
														IntLCD(0);
														IntLCD(year);
													}
													CmdLCD(0x8A);
													CmdLCD(0x0E);

													// ---- Read 1st digit ----
													while (1)
													{
															key = KeyVal();
														
															if (key == 13) // CANCEL pressed BEFORE entering
															{
																	CmdLCD(0x0C);
																	CmdLCD(0x01);
																	goto IN; // Jump to main menu without saving
															}
															if (key >= 0 && key <= 9)
															{
																	y1 = key;
																	CmdLCD(0x0E);
																	CharLCD(y1 + '0');
																	delay_ms(200);
																	break;
															}
													}

													// ---- Read 2nd digit ----
													while (1)
													{
															key = KeyVal();
															if (key >= 0 && key <= 9)
															{
																	y2 = key;
																	CmdLCD(0x0E);
																	CharLCD(y2 + '0');
																	delay_ms(200);
																	break;
															}
													}
													
													// ---- Read 3rd digit ----
													while (1)
													{
															key = KeyVal();
															if (key >= 0 && key <= 9)
															{
																	y3 = key;
																	CmdLCD(0x0E);
																	CharLCD(y3 + '0');
																	delay_ms(200);
																	break;
															}
													}

													// ---- Read 4th digit ----
													while (1)
													{
															key = KeyVal();
															if (key >= 0 && key <= 9)
															{
																	y4 = key;
																	CmdLCD(0x0E);
																	CharLCD(y4 + '0');
																	delay_ms(200);
																	break;
															}
													}
													newYear= (y1 * 1000) + (y2 * 100) + (y3 * 10) + y4;

													// Validate range
													if (newYear < 1)
													{
															CmdLCD(0xC0);
															StrLCD("INVALID!");
															delay_ms(800);
															CmdLCD(0x01);
															CmdLCD(0x80);
															StrLCD("SET YEAR:");
															continue;
													}

													//----------------------------------------------------
													// Ask user to SAVE or EDIT again
													//----------------------------------------------------
													CmdLCD(0xC0);
													StrLCD("SAVE      EDIT");
													delay_ms(300);

													//----------------------------------------------------
													// Wait for SAVE (13) or EDIT (14)
													//----------------------------------------------------
													while (1)
													{
															int K = KeyVal();

															if (K == 14) 											// EDIT - retry
															{
																	CmdLCD(0x01);
																	CmdLCD(0x80);
																	StrLCD("SET YEAR:");
																	break; 											// restart digit entry
															}

															if (K == 13) 									// SAVE key pressed ? update RTC and exit
															{
																// Basic year validation (non-zero positive)
																if(!ValidateRTCDate(date, month, newYear)) 
																{
																		CmdLCD(0x01);
																		CmdLCD(0x80);
																		StrLCD("INVALID MONTH!");
																		UARTTxStr("[WARN] Invalid Date/Month/Year combination!\n\r");
																		delay_ms(1200);
																		CmdLCD(0x01);
																		goto Y1;
																}
																
																year = newYear;
																SetRTCDateInfo(date, month, year);
																CmdLCD(0x0C);
																CmdLCD(0x01);
																goto IN;
															}

															delay_ms(150);
													}
											}
									}								
									
									//------------------------------------------------------------
									// If field == 7 ? DAY of week editing mode
									//------------------------------------------------------------
									else if(field == 7)
									{
										CmdLCD(0x01);       // Clear LCD
										CmdLCD(0x80);       // 1st line start
										StrLCD("SET DAY:");
										
										
										while (1)
										{
											int Key=KeyVal();
												if (Key == 15)      // '+' key pressed - increase day index
												{
														day++;
														if(day==7)			// Wrap from 7 to 0 (SUN)
														{
															day=0;
														}
												}
												else if (Key == 10) // '-' key pressed - decrease day index
												{
													day--;
													if(day < 0)				// Wrap negative to SAT
														day=6;
												}
												
												else if (Key  == 13) // SAVE key pressed - write to RTC
												{
														SetRTCDay(day);
														CmdLCD(0x01);
														goto IN;				// Exit to main menu
												}

												//------------------------------------------------
												// Display updated DAY string on LCD
												//------------------------------------------------
												CmdLCD(0x88);       // 2nd line
												StrLCD("  ");       // small space
												CmdLCD(0x8A);  
												StrLCD((u8*)week[day]);// Show updated minutes

												delay_ms(200);     // small delay to avoid fast repeat
										}	
									}
									
									//------------------------------------------------------------
									// If field == 8 ? EXIT field selection and return
									//------------------------------------------------------------
									else if(field == 8)
									{
										UARTTxStr("*** Time and Date Updated ***\n\r");  // UART confirmation message
										CmdLCD(0x01);
									  goto IN1;																				// Return to edit main menu
									}
								}	
								
								//------------------------------------------------------------
								// If opt == 2 ? Temperature Setpoint edit mode
								//------------------------------------------------------------
								else if (opt == 2)
								{
									int newTemp;
									int press1=0;
										
									if(press1 == 0)
									{
										UARTTxStr("*** Set Point Editing Mode Activated ***\n\r");
											press1++;
									}
									
									// Display "SET TEMP:" prompt on LCD
									T1:
									CmdLCD(0x01);      		 // Clear LCD
									CmdLCD(0x80);       		// 1st line start
									StrLCD("SET TEMP:");
									CmdLCD(0xC0);      			 // 2nd line
									CharLCD(' ');      		 // small space
									DisplayTemp(setpoint); // Show updated setpoint
									CharLCD(0xDF);     		 // Degree symbol
									CharLCD('C');
										

									while (1)
									{
											s8 t1 = -1, t2 = -1;
											int key;

											// Clear previous digits area
											CmdLCD(0x8A);
											StrLCD("  ");
											CmdLCD(0x8C);
											if(hour>9)
											{
												DisplayTemp(setpoint);
											}
											else
											{
												DisplayTemp(0);
												DisplayTemp(setpoint);
											}
											CmdLCD(0x8C);
											CmdLCD(0x0E);
											
											
											// ---- Read 1st digit ----
											while (1)
											{
												key = KeyVal();
												
												if (key == 13) // CANCEL pressed BEFORE entering
												{
														UARTTxStr("*** Set Point Editing Done ***\n\r");
														CmdLCD(0x0C);
														CmdLCD(0x01);
														goto IN1; // exit immediately, no save
												}
											
												if (key >= 0 && key <= 9)
												{
													t1 = key;
													CmdLCD(0x0E);
													CharLCD(t1 + '0');
													delay_ms(200);
													break;
												}
											}

											// ---- Read 2nd digit ----
											while (1)
											{
												key = KeyVal();
												if (key >= 0 && key <= 9)
												{
													t2 = key;
													CmdLCD(0x0E);
													CharLCD(t2 + '0');
													delay_ms(200);
													break;
												}
											}

											newTemp= (t1 * 10) + t2;

											//----------------------------------------------------
											// Ask user to SAVE or EDIT again
											//----------------------------------------------------
											CmdLCD(0xC0);
											StrLCD("SAVE      EDIT");
											delay_ms(300);

											//----------------------------------------------------
											// Wait for SAVE (13) or EDIT (14)
											//----------------------------------------------------
											while (1)
											{
												int K = KeyVal();

												if (K == 14) 										// EDIT - retry
												{
													CmdLCD(0x01);
													goto T1; 											// restart digit entry
												}

												if (K == 13) // SAVE key pressed ? update RTC and exit
												{
													setpoint = newTemp;						// Update setpoint
													DisplayTemp(setpoint);
													UARTTxStr("*** Set Point Editing Done ***\n\r");
													CmdLCD(0x0C);
													CmdLCD(0x01);
													goto IN1;												 
												}

												delay_ms(150);
											}
										}
									}
								
									//------------------------------------------------------------
									// If opt == 3 ? EXIT edit mode completely
									//------------------------------------------------------------
								  else if(opt == 3)
								  {
										u=0;
										UARTTxStr("*** EXITING EDIT MODE ***\n\r");
									  CmdLCD(0x01);
									  goto INPUT;													 // Return to monitoring loop
								  }
							  }							
					    }
			      }
	       }
			 }
