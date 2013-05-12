#ifndef __PULSOUT_C__
#define __PULSOUT_C__

#include "delay-10us.c"

#define PULSOUT(p, us10) \
    p = 1; \
    delay_10us(us10); \
    p = 0;

#endif
