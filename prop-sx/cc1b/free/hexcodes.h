/*
  The following C macro's allow direct coding of instructions.

  Note that direct coded instructions are different from inline
  assembly seen from the compiler. The compiler will view the
  instruction codes as values only and not as instructions. All high
  level properties are lost. The compiler will reset optimization,
  bank updating, etc. after a DW statement.

  Example usage:

  1. In DW statements. 
    #asm
     DW __SLEEP                   // Enter sleep mode
     DW __CLRF(__INDF)            // Clear indirectly
     DW __ANDLW(0x80)             // W = W & 0x80;
     DW __DECF(__FSR,__F)         // Decrement FSR
     DW __BCF(__STATUS,__Carry)   // Clear Carry bit
     DW __GOTO(0)                 // Goto address 0
    #endasm

  2. In cdata statements:
    #pragma cdata[1] = __GOTO(0x1FF)
*/


#ifndef HEX_CODES_H1B
#define HEX_CODES_H1B

#if __CoreSet__ ==  1250  ||  __CoreSet__ ==  1251

#define __INDF   0
#define __TMR0   1
#define __PCL    2
#define __STATUS 3
#define __FSR    4
#define __PORTA  5
#define __PORTB  6
#define __PORTC  7
#define __Carry  0
#define __Zero   2
#define __PD     3
#define __TO     4
#define __PA0    5
#define __PA1    6
#define __F      1
#define __W      0

#define __NOP         0x0000
#define __MOVWF(f)    0x0020+((f)&0x1F)
#define __CLRW        0x0040
#define __CLRF(f)     0x0060+((f)&0x1F)
#define __SUBWF(f,d)  0x0080+((f)&0x1F)+((d&1)<<5)
#define __DECF(f,d)   0x00C0+((f)&0x1F)+((d&1)<<5)
#define __IORWF(f,d)  0x0100+((f)&0x1F)+((d&1)<<5)
#define __ANDWF(f,d)  0x0140+((f)&0x1F)+((d&1)<<5)
#define __XORWF(f,d)  0x0180+((f)&0x1F)+((d&1)<<5)
#define __ADDWF(f,d)  0x01C0+((f)&0x1F)+((d&1)<<5)
#define __MOVF(f,d)   0x0200+((f)&0x1F)+((d&1)<<5)
#define __COMF(f,d)   0x0240+((f)&0x1F)+((d&1)<<5)
#define __INCF(f,d)   0x0280+((f)&0x1F)+((d&1)<<5)
#define __DECFSZ(f,d) 0x02C0+((f)&0x1F)+((d&1)<<5)
#define __RRF(f,d)    0x0300+((f)&0x1F)+((d&1)<<5)
#define __RLF(f,d)    0x0340+((f)&0x1F)+((d&1)<<5)
#define __SWAPF(f,d)  0x0380+((f)&0x1F)+((d&1)<<5)
#define __INCFSZ(f,d) 0x03C0+((f)&0x1F)+((d&1)<<5)
#define __BCF(f,b)    0x0400+((f)&0x1F)+(((b)&7)<<5)
#define __BSF(f,b)    0x0500+((f)&0x1F)+(((b)&7)<<5)
#define __BTFSC(f,b)  0x0600+((f)&0x1F)+(((b)&7)<<5)
#define __BTFSS(f,b)  0x0700+((f)&0x1F)+(((b)&7)<<5)
#define __OPTION      0x0002
#define __SLEEP       0x0003
#define __CLRWDT      0x0004
#define __TRIS(p)     0x0000+((p)&7)
#define __RETLW(k)    0x0800+((k)&0xFF)
#define __CALL(a)     0x0900+((a)&0xFF)
#define __GOTO(a)     0x0A00+((a)&0x1FF)
#define __MOVLW(k)    0x0C00+((k)&0xFF)
#define __IORLW(k)    0x0D00+((k)&0xFF)
#define __ANDLW(k)    0x0E00+((k)&0xFF)
#define __XORLW(k)    0x0F00+((k)&0xFF)

#define __RETFIE      0x000E
#define __RETURN      0x000C

#define __MOVWM       0x0042
#define __MOVMW       0x0043
#define __RETP        0x000D
#define __RETIW       0x000F
#define __SKIP        0x0602
#define __BANK(n)     0x0018+((n)&7)
#define __IREAD       0x0041
#define __MODE(n)     0x0050+((n)&0xF)
#define __PAGE(n)     0x0010+((n)&7)

#endif

#endif
