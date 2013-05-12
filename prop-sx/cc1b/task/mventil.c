// LEANSLICE - MULTITASKING
// VENTILATION CONTROL
// Task ventilationControl: control ventilation on/off, delayed off switching
// Task generatePCM: send PCM pulses depending on ventilation on/off state
// Task generatePulses: send 100 pulses when ventilation goes on or off


#pragma taskOptions 2

enum { TimerVC, TimerPulse, TimerPCM };  // Timer identifiers
#define FreqOsc      4000000  // Oscillator frequenzy
#define FreqTimer    100      // Timer frequency, 100 Hz
#define TimerType    uns16    // uns8, uns16 or uns24
#define NoOfTimers   3        // 1 .. 8
#define DELAY_TYPE   1
#include "delay.h"

enum { Off, On};

bit highTemperature @ PORTA.0;
bit ventilation     @ PORTA.1;
bit outPCM          @ PORTA.2;
bit outPulse        @ PORTA.3;
#define INIT_PORTA 0b00000000
#define INIT_TRISA 0b00000001

Task generatePulses( void)
{
    static char pulses;
    pulses = 100;
    do  {
        outPulse = 1;
        delay( TimerPulse, 50);  // 0.5 seconds
        outPCM = 0;
        delay( TimerPulse, 50);  // 0.5 seconds
    } while (--pulses > 0);
}

Task ventilationControl( void)
{
    ventilation = Off;

    while (!highTemperature)
        waitState();

    ventilation = On;
    startTask( generatePulses);

  VENTILATION_ON:
    while (highTemperature)
        waitState();

    startTimer(TimerVC, 10000); // 100 seconds
    while (!timeout(TimerVC))  {
        if (highTemperature)
            goto VENTILATION_ON;
        waitState();
    }
    startTask( generatePulses);
    restartTask();
}

Task generatePCM( void)
{
 /* NOTE: pulse width may not be correct when
    ventilation change state from 0 to 1 or from 1 to
    0. Ensuring that PCM width is always correct will
    require more code.
 */
    while (1)  {
        outPCM = ventilation;
        delay( TimerPCM, 10);  // 0.1 seconds
        outPCM = !ventilation;
        delay( TimerPCM, 90);  // 0.9 seconds
    }
}


void main( void)
{
    PORTA = INIT_PORTA;
    TRISA = INIT_TRISA;
    startTask( ventilationControl);
    startTask( generatePCM);
    clearTask( generatePulses);
    initTimers();

    while (1)  {
        timerTick();
        taskSlicer();
    }
}
