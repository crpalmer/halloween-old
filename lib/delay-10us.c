#ifndef __DELAY_10US_C__
#define __DELAY_10US_C__

/* This function is off by 1 cycle if you call it with a constant value.
 * That is, delay_10us(100);
 * will cause a delay, including function call overhead, of 100*FREQ_MHZ+1
 * cycles.
 */

void
delay_10us(unsigned char us10)
{
    unsigned char x;

    /* (1+1+(1+1+3)*X+(-4+3)+1+3)*us10+-4+3 + fcall + 1 + Xassign
     * (5+5*X)*us10+1 + fcall + 1 + Xassign
     * ==> (5+5*X) = 10*FREQ_MHZ
     *     X = (10*FREQ_MHZ-5)/5
     *       = 2*FREQ_MHZ - 1
     */
    x = 2*FREQ_MHZ-1 - 2;
#pragma updateBank 0
    W = 2*FREQ_MHZ-1;		/* 1 cycle + 1 for the bank */

pulsout_delay1:
    nop();

pulsout_delay2:
    nop();
    x = decsz(x);
    goto pulsout_delay2;

    x = W;
    us10 = decsz(us10);
    goto pulsout_delay1;

#pragma updateBank 1
}
    
#endif
