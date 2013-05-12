#ifndef __DELAY_RTCC256_C__
#define __DELAY_RTCC256_C__

#ifndef FREQ_MHZ
#error "You must define FREQ_MHZ to be the clock speed in Mhz"
#endif

/* delay_rtcc256(reps, us)
   delay_rtcc256_ms(ms)

   Use the RTCC register with a multiplier of 256 to wait for the specified
   amount of time.  The time is reps*us microseconds.

   These functions require an external resonator to be accurate (the internal
   seems to be off by about 3 seconds a minute).

   This timing is not 100% accurate because it counts in increments of 256
   ticks.  This means that we can be off by up to 128 ticks according to
   our counting.  There is also a random undershoot of 0-254 ticks because
   we count the first as a full 256 when it is on average going to be 128
   ticks.
 */

#define delay_rtcc256_ms(x) delay_rtcc256(x, 1000)

void
delay_rtcc256(uns16 reps, uns16 us)
{
    uns16 acc = -128;

    us = us * FREQ_MHZ;
    while (reps > 0) {
	unsigned char rt = RTCC;
	while (rt == RTCC) {}

	acc += 256;
	while (acc >= us) {
	    acc -= us;
	    if (reps > 0) reps --;
	}
    }
}

#endif
