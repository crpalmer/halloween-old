/*
  ----------------------------------------------------------
  TIMING/DELAY LIBRARY FOR LEANSLICE MULTITASKING

  Copyright (c) B Knudsen Data, Trondheim, Norway, 1999-2002
  ----------------------------------------------------------

  ==> The timer module (delay.h) implements 8, 16 or 24 bit software
      timers that can be used in the tasks to get precise delays.
      The timer increments must of course have to be much slower
      than the instruction cycles. This means that the software
      timer operates around 100 or 1000 Hz when the microcontroller
      run at 4 - 20 MHz. Around 1000 instruction cycles for each 
      software timer decrement is normally sufficient. Note that 
      math operations are time consuming. It is possible to modify 
      module delay.h if the actual timing requirements is outside 
      the implemented options.

  The module delay.h offers both simple delays or active waiting. It
  is possible to define a timer for each task.

  Required definitions in the application program:

   enum {TimerA, TimerB};     // Timer identifiers
   #define FreqOsc      4000000 // Oscillator frequenzy
   #define FreqTimer    1000    // Timer frequency, 1000 or 100 Hz
   #define TimerType    uns16   // uns8, uns16 or uns24
   #define NoOfTimers   2       // 1 .. 8
   #define DELAY_TYPE   1
   #include "delay.h"

  Initialization and increments:

   initTimers();  // initialization on application startup

   // The timers need to be incremented by making frequent calls
   // to timerTick()in main. The calls must not be more infrequent 
   // than the timer decrement period 
   timerTick();

  Using the timers by active waiting or simple delays:

   // active waiting means starting the timer and checking for timeout:
   startTimer(TimerA, 100);
   if (timeout(TimerA)) ..

   // Simple delays with built-in task switching until timeout:
   delay(TimerA, 1000); // delay up to 1000 timer ticks

  Note that delay(1) will not be precise because the timer
  decrements depends on a single hardware timer, and it is not
  possible to tell when the next increment occurs. The duration of
  delay(1) will be in the interval 0.0 to 1.0 period. For example,
  if half of the period to the next hardware timer increment have
  expired, then the actual period will be 0.5. This means that a
  series of delays will be more precise than a single delay. The
  reason for this is that several task may need service at a
  timeout, and some tasks may have to wait a fraction of the next
  timer increment before the task is scheduled. On the other hand,
  if the timer is started because of an external IO event, then the
  duration of delay(100) will be in the interval 99.0 - 100.0.

  The software timers is implemented using the 8 bit hardware timer
  TMR0 and the prescaler. Up to 8 timers is allowed.

  timerTick() : 23+ instruction words. The size increases depending
  on the timer size (8, 16 or 24 bit) and the number of timers.
  Three 16 bit timers requires 46 instruction words. The function
  executes in minimum 12 instructions cycles when the next hardware
  timeout period have not expired. This increases when software
  timers needs to be decremented (around 49 instruction cycles when
  decrementing three active 16 bit timers). Required definitions in
  the application program:


  FreqOsc / (4*FreqTimer) = Cycles = PrescalerDivide * MaxTimerCount
 20000000 / (4*1000)      = 5000   = 32              * 156.25
 16000000 / (4*1000)      = 4000   = 32              * 125
 12000000 / (4*1000)      = 3000   = 16              * 187.5
 10000000 / (4*1000)      = 2500   = 16              * 156.25
  8000000 / (4*1000)      = 2000   = 16              * 125
  4000000 / (4*1000)      = 1000   =  8              * 125
  4000000 / (4*100)       = 10000  = 64              * 156.25
  2000000 / (4*1000)      = 500    =  4              * 125
  2000000 / (4*100)       = 5000   = 32              * 156.25
  1000000 / (4*100)       = 2500   = 16              * 156.25
   400000 / (4*100)       = 1000   =  8              * 125
   100000 / (4*100)       = 250    =  2              * 125
    32768 / (4*20)        = 409.6  =  2              * 204.8
    32768 / (4*10)        = 819.2  =  4              * 204.8

  'Cycles' telles how many instructions can be executed between
  each timer increment.
*/

#if !defined FreqOsc  ||  !defined FreqTimer
 #error FreqOsc and FreqTimer must be properly defined
#endif


#if FreqOsc % (4*FreqTimer) != 0
 #if FreqOsc == 32768  &&  FreqTimer == 10
  #define PrescalerDivide   1
  #define MaxTimerCount 204
  #define TimerDecimals 205
 #elif FreqOsc == 32768  &&  FreqTimer == 20
  #define PrescalerDivide   0
  #define MaxTimerCount 204
  #define TimerDecimals 205
 #else
  #error Combination of FreqOsc and FreqTimer is not implemented
 #endif
#else
 #if FreqOsc / (4*FreqTimer) == 250
  #define PrescalerDivide   0
  #define MaxTimerCount 125
 #elif FreqOsc / (4*FreqTimer) == 500
  #define PrescalerDivide   1
  #define MaxTimerCount 125
 #elif FreqOsc / (4*FreqTimer) == 1000
  #define PrescalerDivide   2
  #define MaxTimerCount 125
 #elif FreqOsc / (4*FreqTimer) == 2000
  #define PrescalerDivide   3
  #define MaxTimerCount 125
 #elif FreqOsc / (4*FreqTimer) == 4000
  #define PrescalerDivide   4
  #define MaxTimerCount 125
 #elif FreqOsc / (4*FreqTimer) == 8000
  #define PrescalerDivide   5
  #define MaxTimerCount 125
 #elif FreqOsc / (4*FreqTimer) == 3000
  #define PrescalerDivide   3
  #define MaxTimerCount 187
  #define TimerDecimals 0x80
 #elif FreqOsc / (4*FreqTimer) == 2500
  #define PrescalerDivide   3
  #define MaxTimerCount 156
  #define TimerDecimals 0x40
 #elif FreqOsc / (4*FreqTimer) == 5000
  #define PrescalerDivide   4
  #define MaxTimerCount 156
  #define TimerDecimals 0x40
 #elif FreqOsc / (4*FreqTimer) == 10000
  #define PrescalerDivide   5
  #define MaxTimerCount 156
  #define TimerDecimals 0x40
 #else
  #error Combination of FreqOsc and FreqTimer is not implemented
 #endif
#endif



#ifndef DELAY_TYPE
 #error The DELAY_TYPE have to be defined (1,..)
#endif




// -------------------------------------------
#if DELAY_TYPE == 1

TimerType softTimer[NoOfTimers];
char tTimeout;
char previousTMR0, subClock;


#define initTimers() \
{ \
    tTimeout = 0xFF; \
    previousTMR0 = 0; \
    subClock = 0; \
    OPTION = PrescalerDivide; \
}

#define startTimer(TimerNo, count) \
{ \
    softTimer[TimerNo] = count; \
    tTimeout.TimerNo = 0; \
}

#define timeout(TimerNo)  tTimeout.TimerNo

#define delay(TimerNo, count) \
{ \
    softTimer[TimerNo] = count; \
    tTimeout.TimerNo = 0; \
    while (!tTimeout.TimerNo) \
        waitState(); \
}


void timerTick( void)
/*
 - decrements active software timers
*/
{
    char sample = TMR0;  // sampling the timer
    subClock -= sample - previousTMR0;
    previousTMR0 = sample;
    if ( Carry)
        return;

    /* new timer tick */
    subClock += MaxTimerCount;

   #ifdef TimerDecimals
    static char decim;
    decim += TimerDecimals;
    if (Carry)
        subClock ++;
   #endif

  #if NoOfTimers <= 3

    if ( !tTimeout.0)  {
        softTimer[0] -= 1;
        if ( softTimer[0] == 0)
            tTimeout.0 = 1;
    }

   #if NoOfTimers >= 2
    if ( !tTimeout.1)  {
        softTimer[1] -= 1;
        if ( softTimer[1] == 0)
            tTimeout.1 = 1;
    }
   #endif

   #if NoOfTimers >= 3
    if ( !tTimeout.2)  {
        softTimer[2] -= 1;
        if ( softTimer[2] == 0)
            tTimeout.2 = 1;
    }
   #endif

  #elif sizeof( TimerType) <= 3  &&  NoOfTimers <= 8

    char i = NoOfTimers;
    char mask = 1;
    FSR = &softTimer[0] % 256;
   #if __IRP_RAM__
    IRP = &softTimer[0] / 256;
   #endif

    do  {
        if ( (mask & tTimeout) == 0)  {
            INDF -= 1;
           #if sizeof( TimerType) >= 2
            if ( INDF == 255)  {
                FSR ++;
                INDF --;
               #if sizeof( TimerType) >= 3
                if ( INDF == 255)  {
                    FSR ++;
                    INDF --;
                    FSR --;
                }
               #endif
                FSR --;
            }
           #endif

            W = INDF;
          #if sizeof( TimerType) >= 2
            FSR ++;
            W |= INDF;
           #if sizeof( TimerType) >= 3
            FSR ++;
            W |= INDF;
           #endif
          #endif
            if ( W == 0)
                tTimeout |= mask;
          #if sizeof( TimerType) >= 2
            FSR --;
           #if sizeof( TimerType) >= 3
            FSR --;
           #endif
          #endif
        }
        FSR += sizeof( TimerType);
        mask <<= 1;
    } while ( --i > 0);

  #else
   #error Not implemented
  #endif
}


#endif
