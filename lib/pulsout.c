#ifndef __PULSOUT_C__
#define __PULSOUT_C__

#define PULSOUT(p, us10) \
    p = 1; \
    pulsout_delay(us10); \
    p = 0;

void
pulsout_delay(unsigned char us10)
{
    unsigned char X;

    /* (1+1+(1+1+3)*X+(-4+3)+1+3)*us10+-4+3 + fcall + 1 + Xassign
     * (5+5*X)*us10+1 + fcall + 1 + Xassign
     * ==> (5+5*X) = 10*FREQ_MHZ
     *     X = (10*FREQ_MHZ-5)/5
     *       = 2*FREQ_MHZ - 1
     */
    X = 2*FREQ_MHZ-1;

/*<?asm
    mov	  w, X
pulsout_delay1:
    nop                 ; 1 cycle                                 -+
    mov	  X, W		; 1 cycle                                  |
pulsout_delay2:         ;                                          |
    nop			; 1 cycle                  -+              |
    decsz X		; 1 cycle, 3 at end         | X            |  us10
    jmp   pulsout_delay2; 3 cycle                  -+              |
    decsz us10		; 1 cycle, 3 at end                        |
    jmp   pulsout_delay1; 3 cycle                                 -+
?>*/
}
    
#endif
