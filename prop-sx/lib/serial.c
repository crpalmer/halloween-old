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

#ifndef SERIAL_TX_PIN
#error "You must define SERIAL_TX_PIN"
#endif

#ifndef SERIAL_FUNCTION_NAME_PREFIX
#define SERIAL_FUNCTION_NAME_PREFIX
#endif

#define SERIAL_DELAY_10US	(100*1000 / SERIAL_BAUD)

#if SERIAL_DELAY_10US <= 255
#define SERIAL_DELAY() delay_10us(SERIAL_DELAY_10US)
#else
#if SERIAL_DELAY_10US > 255*2
#error "Oops, I didn't see this coming."
#endif
#define SERIAL_DELAY() \
	do { delay_10us(255); delay_10us(SERIAL_DELAY_10US-255); } while(0)
#endif
	

#define SERIAL_FUNCTION_NAME(x) SERIAL_FUNCTION_NAME_PREFIX##x
#define SERIAL_INIT SERIAL_FUNCTION_NAME(serial_init)
#define SERIAL_TX_BYTE SERIAL_FUNCTION_NAME(serial_tx_byte)
#define SERIAL_TX_HEX SERIAL_FUNCTION_NAME(serial_tx_hex)
#define SERIAL_TX_STRING SERIAL_FUNCTION_NAME(serial_tx_string)
#define SERIAL_RX_BYTE SERIAL_FUNCTION_NAME(serial_rx_byte)

void
SERIAL_INIT(void)
{
    SERIAL_TX_PIN = 1;
    delay_loop(100);
}

void
SERIAL_TX_BYTE(uns8 byte)
{
    uns8 bits = 10;

#if DEBUG
lcd_serial_tx_byte(byte);
#endif

    SERIAL_TX_PIN = 0;
    for (;;) {
	SERIAL_DELAY();
	if (bits == 0) return;
	bits--;
        SERIAL_TX_PIN = byte.0;
	/*<?asm
	STC
	?>*/
        byte = rr(byte);
    }
}

void
SERIAL_TX_HEX(uns8 hex)
{
    uns8 high = hex >> 8;
    hex = hex & 0x0f;
    if (high >= 10) high += 'A';
    else high += '0';
    if (hex >= 10) hex += 'A';
    else hex += '0';
    SERIAL_TX_BYTE(high);
    SERIAL_TX_BYTE(hex);
}

void
SERIAL_TX_STRING(const char *str)
{
    while (*str) {
	SERIAL_TX_BYTE(*str);
	str++;
    }
}

uns8
SERIAL_RX_BYTE(void)
{
    uns8 bits = 0;
    uns8 result = 0;

    do {
	while (SERIAL_RX_PIN == 1) {}
	delay_10us(SERIAL_DELAY_10US / 2);
    } while (SERIAL_RX_PIN == 1);

#if DEBUG
lcd_serial_tx_byte('!');
#endif
    for (;;) {
	SERIAL_DELAY();
	if (bits == 8) break;
	bits++;
	if (SERIAL_RX_PIN) {
	   /*<?asm
	   STC
	   ?>*/
	} else {
	   /*<?asm
	   CLC
	   ?>*/
	}
	result = rl(result);
    }

#if DEBUG
lcd_serial_tx_hex(result);
#endif

    return result;
}

#undef SERIAL_DELAY
#undef SERIAL_DELAY_10US
