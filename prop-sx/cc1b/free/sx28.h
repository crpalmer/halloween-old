// HEADER FILE
#pragma chip SX28, core 12 sx, code 2048, ram 8 : 0xFF

/* RAM ADDRESS    BANKED  INDIRECT
   0x000 - 0x007  no      yes    : special purpose registers
   0x008 - 0x00F  no      yes    : unbanked RAM
   0x010 - 0x01F  yes     yes    : RAM, bank 0 (16 byte)
   0x030 - 0x03F  yes     yes    : RAM, bank 1 (16 byte)
   0x050 - 0x05F  yes     yes    : RAM, bank 2 (16 byte)
   0x070 - 0x07F  yes     yes    : RAM, bank 3 (16 byte)
   0x090 - 0x09F  yes     yes    : RAM, bank 4 (16 byte)
   0x0B0 - 0x0BF  yes     yes    : RAM, bank 5 (16 byte)
   0x0D0 - 0x0DF  yes     yes    : RAM, bank 6 (16 byte)
   0x0F0 - 0x0FF  yes     yes    : RAM, bank 7 (16 byte)
*/

/* PREDEFINED:

  char W;
  char INDF, RTCC, PC, STATUS, FSR, RA, RB, RC;
  char INDF0, TMR0, PCL, PORTA, PORTB, PORTC; // optional
  char OPTION;

  /* STATUS : */   bit Carry, DC, Zero_, PD, TO, PA0, PA1, PA2;

  char MODE;
  char DDRA, PLP_A, LVL_A;
  char DDRB, PLP_B, LVL_B, ST_B, WKEN_B, WKED_B, WKPND_B, CMP_B;
  char DDRC, PLP_C, LVL_C, ST_C;
  char TRISA, TRISB, TRISC; // optional
*/
