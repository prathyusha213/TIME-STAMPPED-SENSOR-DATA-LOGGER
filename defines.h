/*------------------------------------------------------------
File    : defines.h
Purpose : Define reusable bit manipulation and write/read macros
          for LPC2129 ARM7 embedded mini project.
------------------------------------------------------------*/

#define SETBIT(WORD,BP) WORD|=1<<BP																						// Set bit BP in register/variable WORD
#define CLRBIT(WORD,BP) WORD&=~(1<<BP)																				// Clear bit BP in register/variable WORD
#define CPLBIT(WORD,BP) WORD^=(1<<BP)																					// Complement/Toggle bit BP in WORD
#define WRITEBIT(WORD,BP,BIT) WORD=((WORD&~(1<<BP))|(BIT<<BP))								// Write a single BIT (0/1) at bit position BP in WORD
#define WRITENIBBLE(WORD,SBP,NIBBLE) WORD=((WORD&~(0xf<<SBP))|(NIBBLE<<SBP))	// Write 4-bit nibble starting at shift bit position SBP
#define WRITEBYTE(WORD,SBP,BYTE) WORD=((WORD&~(0xff<<SBP))|(BYTE<<SBP))				// Write 8-bit BYTE value into WORD at shift position SBP
#define WRITEHWORD(WORD,SBP,HWORD) WORD=((WORD&~(0xffff<<SBP))|(HWORD<<SBP))	// Write 16-bit half-word HWORD into WORD at shift position SBP
#define READBIT(reg, bit)   ((reg >> bit) & 1U)																// Read single bit status from register reg at position bit	

