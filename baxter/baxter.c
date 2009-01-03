#pragma chip SX28 // select device

#pragma asmDir:	DEVICE OSCHS3, TURBO, STACKX, OPTIONX
#pragma asmDir:	IRC_CAL IRC_FAST
#pragma asmDir:	FREQ	50_000_000
#pragma asmDir:	ID	'leds'
#define FREQ_MHZ 50


/* IO PORT DEFINITIONS */

bit HEAD @ RC.0;
bit TAIL @ RC.1;
bit ATTACK_BUTTON @ RB.6;


#pragma rambank 0
#pragma origin 0   // interrupt must be located first on codepage 0

interrupt iServer(void)
{
    /* NOTE: W, STATUS and FSR are automatically saved to
       and restored from shadow registers */
}



/* Included libraries */

#pragma rambank 1

#include "delay-rtcc256.c"
#include "pulsout.c"
#include "random.c"

/* Main code */

#pragma codepage 0
#pragma rambank 0

#define HEAD_SPEED  20
#define HEAD_TO	     4

void
main(void)
{
    unsigned char head_at, head_to, head_dir = 1, head_top, head_bottom;
    uns16 playing;

    clearRAM();  // built in function
	
    OPTION = 0x87;		// enable interrupts for timer

    TRISC = 0xfc;
    TRISB = 0xe0;
    random_seed = 45677;

    while (1) {

	head_at = 150;
	PULSOUT(HEAD, head_at);
	delay_rtcc256_ms(20);

	while (! ATTACK_BUTTON) { random(); }
	while (ATTACK_BUTTON) { random(); }

	head_to = random() % HEAD_TO;
 	head_to += HEAD_TO / 2;
	head_to += 1;
	if (head_to > HEAD_TO) head_to = HEAD_TO;
	head_to = head_to * HEAD_SPEED;
	head_top = 150 + head_to;
	head_bottom = 150 - head_to;
	
	playing = random() % 512;
	playing += 512;
	while (--playing) {
	    if (head_dir) head_at += HEAD_SPEED;
	    else head_at -= HEAD_SPEED;
	    RB = head_at - 150;
	    if (head_at < head_bottom) {
		head_dir = 1;
		head_at = head_bottom;
	    } else if (head_at > head_top) {
		head_dir = 0;
		head_at = head_top;
	    }
	    PULSOUT(HEAD, head_at);
	    delay_rtcc256_ms(20);
	}
    }
}

