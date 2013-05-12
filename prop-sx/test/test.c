#pragma chip SX28 // select device

#pragma asmDir:	DEVICE OSCHS3, TURBO, STACKX, OPTIONX
#pragma asmDir:	IRC_CAL IRC_FAST
#pragma asmDir:	FREQ	50_000_000
#pragma asmDir:	ID	'leds'
#define FREQ_MHZ 50


/* IO PORT DEFINITIONS */


bit pinB0 @ RB.0;
bit pinB1 @ RB.1;
unsigned char interrupts;


// GLOBAL VARIABLE DEFINITIONS

#pragma rambank 0


#pragma origin 0   // interrupt must be located first on codepage 0

interrupt iServer(void)
{
    /* NOTE: W, STATUS and FSR are automatically saved to
       and restored from shadow registers */

interrupts++;

#if 0
    if (pinB1) pinB1 = 0;
    else pinB1 = 1;
#else
    nop();
#endif

}



// ************************************************
// ************************************************
// INCLUDED C FILES AND MATH ROUTINES

#pragma codepage 1

#if 0
#include "delay-rtcc256.c"
#endif

#include "delay-loop.c"

#define SERIAL_RX_PIN RC.0
#define SERIAL_TX_PIN RC.0
#define SERIAL_FUNCTION_NAME_PREFIX lcd_
#define SERIAL_BAUD 19200
#include "serial.c"

#pragma codepage 2

#undef SERIAL_RX_PIN
#undef SERIAL_TX_PIN
#undef SERIAL_FUNCTION_NAME_PREFIX
#undef SERIAL_BAUD
#define SERIAL_RX_PIN	RC.7
#define SERIAL_TX_PIN	RC.6
#define SERIAL_BAUD	19200
#define DEBUG 1
#include "vmusic2.c"

#pragma codepage 0

void
main(void)
{
#if 1
    clearRAM();
    OPTION = 0x87;

    TRISB = 0;
    TRISC = (1<<7)|(1<<5)|(1<<4)|0xe;

    RB = 0;

    lcd_serial_init();
    lcd_serial_tx_byte(0x0b);
    delay_loop(20);
    lcd_serial_tx_byte(0x0c);
    delay_loop(20);
    lcd_serial_tx_byte(0x11);
    delay_loop(20);

    lcd_serial_tx_string("Init.");

    vmusic2_init();

    lcd_serial_tx_string("Volume.");

    vmusic2_volume(0x00, 1);
    vmusic2_play("0", 1);
    vmusic2_wait_for_prompt();

#elif 0
    unsigned char i;

    clearRAM();
    OPTION = 0x87;

    TRISC = 0x7f;

    serial_init();
    serial_tx_byte(0x0b);
    delay_loop(500);
    serial_tx_byte(0x11);
    delay_loop(500);
loop:
    for (i = 'A'; i <= 'Z'; i++) {
	serial_tx_byte(i);
	serial_tx_string("0123456789012345");
	delay_loop(1000);
    }
    goto loop;
    RB.3 = 1;

   
#else
    bank0 unsigned char hold;
    bit holdB0 @ hold.0;	
    bit holdB3 @ hold.3;
    uns16 i;

    clearRAM();  // built in function
	
    OPTION = 0x87;

#if 0
    TRISB = 0;
    RB = 0;
    RB.4 = 1;
    while (1) {
	holdB0 = pinB0;
	hold++;
	pinB0 = holdB0;
	for (i = 0; i < 1000; i++) pulsout_delay(100);
    }
#else
    unsigned char pos;

    TRISB = 0;
    TRISC = 0xfe;
    RC = 0;
    RB = 0;

    while (1) {
#if 1
	for (pos = 70; pos <= 230; pos += 2) {
	    PULSOUT(RC.0, pos);
	    delay_rtcc256_ms(20);
	}
	for (pos = 230; pos >= 70; pos -= 2) {
	    PULSOUT(RC.0, pos);
	    delay_rtcc256_ms(20);
	}
#else
	    PULSOUT(RC.0, 150);
	    delay_rtcc256_ms(20);
#endif
    }
#endif

#endif
}

