// sertstc.c : demo pgm for SX-28 RS-232
// uses sxio.h
//
//                 modified 09/04/2007 to use new CC1B 0.7A features
//
#pragma chip SX28 // select device

// SX-KEY specific settings:
/*<?asm
	DEVICE	turbo
	DEVICE	optionx
	DEVICE	stackx
	DEVICE	OSCHS3
	IRC_CAL	IRC_FAST
	FREQ	50_000_000
;	RESET   main
?>*/

// ************************************************
// ************************************************
// MACRO DEFINITIONS AND CONSTANTS

// ************************************************
// ************************************************
// IO PORT DEFINITIONS

// ************************************************
// ************************************************
// GLOBAL VARIABLE DEFINITIONS


// ************************************************
// ************************************************
// PROTOTYPES AND CODEPAGES

// ************************************************
// ************************************************
// INTERRUPT SUPPORT

#pragma origin 0   // interrupt must be located first on codepage 0

// ************************************************
// ************************************************
// INCLUDED C FILES AND MATH ROUTINES

#include "sxio.h"

#pragma codepage 0


// ************************************************
// ************************************************
// C FUNCTIONS

void main(void)
{

    static const char msg[] =
    	"\n\rHello SX C World\n\rpush 'a' to loop again\n\r";
	char ch;
	
    OPTION = 0xa8;  // ints off!

    clearRAM();  // built in function

	// not needed..
#if 0
    // configure 8-bit RB, all outputs
	TRISB = 0x00;   // all pins output
	LVL_B = 0xff;   // all TTL voltages
    ST_B = 0xff;    // schmitt triggers off
    PLP_B = 0xff;   // pull-ups off
    WKED_B = 0xff;  // Wake up falling edge
    WKPND_B = 0x00; // Wake up clear register
    WKEN_B = 0xff;  // Wake up disabled

    // configure 8-bit RC, all outputs
	TRISC = 0x00;   // all pins output
	LVL_C = 0xff;   // all TTL voltages
    ST_C = 0xff;    // schmitt triggers off
    PLP_C = 0xff;   // pull-ups off
#endif

    serial_on(100);	// turn on serial I/O, delay appox. 100 milli seconds
	                // the delay gives the SX some stablilization
					// time
	
	while (1) {
	
        puts(msg);
		putchar('>');
		ch = getchar();
		if (ch != 'a') break;
	}	

    serial_off();	// turn off serial, calling this is important in the event
	                // that the transmit buffer is not yet processed by the
					// interrupt handler in sxio.h
	
	sleep();

}
// end of sertstc.c
