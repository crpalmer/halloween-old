#ifndef __DELAY_C__
#define __DELAY_C__

#ifndef FREQ_MHZ
#error "You must define FREQ_MHZ to be the clock speed in Mhz"
#endif

void
delay(uns16 ms)
{
    while (ms > 0) {
	/* 17 instructions per loop */
	unsigned char loops_per_ms = FREQ_MHZ;
	while (loops_per_ms > 0) {
	    /* 12 cycles for each loop here */
	    unsigned char x;

	    for (x = 0; x < 99; x++) {
		/* 10 cycles for each loop */
		nop();
	    }
	    loops_per_ms--;
	}
	ms--;
    }
}

#endif
