/* serial I/O
 *
 * This library can be included multiple times to generate serial
 * code for multiple pins.
 *
 * #define SERIAL_TX_PIN xxx
 * #define SERIAL_RX_PIN xxx
 * #define SERIAL_BAUD xxx
 * #define SERIAL_FUNCTION_NAME_PREFIX foo_
 * #undef SERIAL_TX_PIN
 * #undef SERIAL_RX_PIN
 * #undef SERIAL_BAUD
 * #undef SERIAL_FUNCTION_NAME_PREFIX
 *
 * and then you can use foo_serial_* as the function names.
 */

#include "pulsout.c"

#ifndef FREQ_MHZ
#error "You must define FREQ_MHZ to be the clock speed in Mhz"
#endif

#ifndef SERIAL_BAUD
#error "You must define SERIAL_BAUD to be the desired baud rate"
#endif

/* I want to write
 
   ((FREQ_MHZ * 1000 * 1000 / SERIAL_BAUD) - 2) / 4
 */

#define SERIAL_DELAY_LOOPS \
	((FREQ_MHZ * 250 * 10) / (SERIAL_BAUD/100) - 1)

#ifndef SERIAL_TX_PIN
#error "You must define SERIAL_TX_PIN"
#endif

#ifndef SERIAL_FUNCTION_NAME_PREFIX
#define SERIAL_FUNCTION_NAME_PREFIX
#endif

#define SERIAL_FUNCTION_NAME(x) SERIAL_FUNCTION_NAME_PREFIX##x
#define SERIAL_TX_BYTE SERIAL_FUNCTION_NAME(serial_tx_byte)
#define SERIAL_TX_STRING SERIAL_FUNCTION_NAME(serial_tx_string)

void
SERIAL_TX_BYTE(uns8 byte)
{
    uns8 bits = 10;
    uns8 x;

#pragma updateBank 0
    SERIAL_TX_PIN = 0;
    do {
	x = SERIAL_DELAY_LOOPS;
loop:
	x--;
	if (x) goto loop;	/* 4 per loop + 2 */
        SERIAL_TX_PIN = byte.0;	/* 4 cycles */
	/*<?asm
	STC			; 1 cycle
	?>*/
        byte = rr(byte);	/* 1 cycle */
    } while (--bits > 0);	/* 4 cycles + 2 to exit */
#pragma updateBank 1
}

void
SERIAL_TX_STRING(const char *str)
{
    while (*str) {
	SERIAL_TX_BYTE(*str);
	str++;
    }
}
