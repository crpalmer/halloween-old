// sxio.h:  Interrupt driven serial I/O of the SX and CC1B
// version: 08/11/2007, included with C4SX, Version 02

#pragma library 1

// ************************************************
// ************************************************
// MACRO DEFINITIONS AND CONSTANTS

#message note: sxio.h set to 19.2 kb (@ 50 MHz), variables in rambank 1

// for 19,200 baud @ 50 MHz
#define BAUD_RATE_192 12
#define START_DELAY_192 19
#define INT_PERIOD 217

// ************************************************
// ************************************************
// IO PORT DEFINITIONS

bit rts_pin @ RA.0;		// not used
bit cts_pin @ RA.1;		// not used
bit rx_pin  @ RA.2;		// receive
bit tx_pin  @ RA.3;		// transmite

// ************************************************
// ************************************************
// GLOBAL VARIABLE DEFINITIONS

#pragma rambank -
unsigned char flags;
bit rx_flag @ flags.1;
bit rx_bit @ flags.2;

// for this version, force there fields to rambank one due to CC1B bug
#pragma rambank 1

unsigned char tx_high;
unsigned char tx_low;
unsigned char tx_count;
unsigned char tx_divide;
unsigned char rx_count;
unsigned char rx_divide;
unsigned char rx_byte;

#pragma location -

// ************************************************
// ************************************************
// INTERRUPT SUPPORT


#pragma origin 0   // interrupt must be located first on codepage 0
#pragma codepage 0

interrupt iServer(void)
{
    /* NOTE: W, STATUS and FSR are automatically saved to
       and restored from shadow registers */

#if 1
    // handle the interrupt
    // transmit code
    tx_divide--;
    if (tx_divide == 0)
    {
        tx_divide = BAUD_RATE_192;
        if (tx_count > 0)
        {
            Carry = 0;      // ready stop bit
            tx_high = rr(tx_high);    // shift to next bit
            tx_low = rr(tx_low);     // both bytes
            tx_count--;     // dec bit count
            if (tx_low.6)
            {
                tx_pin = 0;  // if bit on, send zero
            } else {
                tx_pin = 1;  //  if bit off, send 1
            }
        }
    }
    // receive code
    rx_bit = rx_pin;
    if (rx_count == 0)           // currently receiving byte?
    {                       // no
        if (rx_bit == 0)
        {
            rx_count = 9;   // if start bit present
        }
        rx_divide = START_DELAY_192;
    }
    rx_divide--;
    if (rx_divide == 0)
    {
        rx_divide = BAUD_RATE_192;
        rx_count--;
        if (rx_count > 0)
        {
            Carry = rx_bit;
            rx_byte = rr(rx_byte);    // process bit in Carry
        } else {
            rx_flag = 1;
        }
    }

    // special return, need to adjust period to next interrupt
    W = -INT_PERIOD;
    retiw();
#endif
}

// ************************************************
// ************************************************
// INCLUDED C FILES AND MATH ROUTINES

// ************************************************
// ************************************************
// C FUNCTIONS

// approximate milliseconds
void delay_milliseconds( char mills )
	{
	char	a ;
	char	b ;

	for ( ; (--mills) ; )
	  for ( a = 25 ; (--a) ; )
	    for ( b = 250 ; (--b) ; )
		{
		}
}	/*  end of 'delay_milliseconds'  */

void serial_on (int mills)
{
    // configure 4-bit RA -- used for serial I/O

	// LVL_A = 0xff;   // all TTL voltages
    LVL_A = 0x00;   // all CMOS voltages
	PLP_A = 0xff;   // pull-ups off
    TRISA = 0x07;   // %00000111 -- pin 3 output, others input

   // this routine depends on memory being zeroed out before
   // being called ...
    tx_divide = BAUD_RATE_192;
    tx_count = 0;
    rx_count = 0;
    tx_pin = 1;     // pre-set bit so 'start' bit has edges

	// enable interrupts
	OPTION = 0x88;  // RTCC_ON | RTCC_PS_OFF
                    // location 0x01 is RTCC, %1000000 bit is on
                    // interrupts on,  %0100000 bit is off
                    // prescaler assigned to WTC, %00001000 bit is on
    if (mills > 0) {
        delay_milliseconds(mills);
    }

}

void serial_off()
{

    while (tx_count) {} // wait until any characters sent

    OPTION = 0xa8;  // ints off!
}

// Windows style _kbhit function
int _kbhit()
{
    if (rx_flag == 0) {return 0;} else {return 1;}
}

int getchar()
{
    while (rx_flag == 0) {}; // wait until rx_flag != 0
    rx_flag = 0;
    return (rx_byte);
}

int putchar(int c_in)
{
    while (tx_count) {}; // wait until tx_count is zero
    tx_high = ~c_in;       // put inverted data in play
    tx_low.7 = 1;       // set start bit
    tx_count = 10;      // start bit, 8 bits of data, and 1 stop

    return(c_in);
}

int puts(const char *in_str)
{
    while (*in_str != 0) {
        putchar(*in_str);
        in_str++;
    }
    return(0);
}

#pragma library 0

// end of sxio.h
