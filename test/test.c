#pragma chip SX28 // select device

#pragma asmDir:	DEVICE OSC4MHZ, TURBO, STACKX, OPTIONX
#pragma asmDir:	IRC_CAL IRC_FAST
#pragma asmDir:	FREQ	4_000_000
#pragma asmDir:	ID	'leds'
#define FREQ_MHZ 4


/* IO PORT DEFINITIONS */


bit pinB0 @ RB.0;



// GLOBAL VARIABLE DEFINITIONS

#pragma rambank 0


#pragma origin 0   // interrupt must be located first on codepage 0

interrupt iServer(void)
{
    /* NOTE: W, STATUS and FSR are automatically saved to
       and restored from shadow registers */

    nop();
}



// ************************************************
// ************************************************
// INCLUDED C FILES AND MATH ROUTINES

#include "delay.c"

#pragma codepage 0

void
main(void)
{
    clearRAM();  // built in function
#if 0
	
	    // configure 8-bit RB, all outputs
	TRISB = 0x00;   // all pins output
	LVL_B = 0xff;   // all TTL voltages
    ST_B = 0xff;    // schmitt triggers off
    PLP_B = 0xff;   // pull-ups off
    WKEN_B = 0xff;  // Wake up disabled
    WKED_B = 0xff;  // Wake up falling edge

    // configure 8-bit RC, all outputs
	TRISC = 0x00;   // all pins output
	LVL_C = 0xff;   // all TTL voltages
    ST_C = 0xff;    // schmitt triggers off
    PLP_C = 0xff;   // pull-ups off

    RB = 0xff;      // all LEDs off!
	
#endif
	
	bank0 unsigned char hold;
	bit holdB0 @ hold.0;	
	// bank1 char c;

	TRISB = 0;   // %1111 1110
	RB = 1;
	while (1) {
	  delay(1000);
	  holdB0 = pinB0;
	  hold++;
	  pinB0 = holdB0;
	}

}

