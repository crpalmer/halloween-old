
; CC1B Ver 0.7B beta, Copyright (c) B Knudsen Data
; C compiler for the Ubicom SX family
; ************  28. Dec 2008   8:00  *************

        device SX28

;INDF        EQU   $00
;FSR         EQU   $04
Zero_       EQU   2
;RB          EQU   $06
pinB0       EQU   0
Count1      EQU   $50
Count2      EQU   $51
Count3      EQU   $52
hold        EQU   $10
holdB0      EQU   0
	DEVICE OSCHS3

  ; FILE test.c
                        ;// ledc28.c:  blink led on RB pin 0 -- simple SX C program for CC1B compiler
                        ;//                 C program similar to led28.src (assemlber for SX)
                        ;//                 modified 07/30/2007 to include SX-Key directives
                        ;//                 modified 09/02/2007 to use new CC1B 0.7A features
                        ;
                        ;#pragma chip SX28 // select device
                        ;
                        ;#pragma asmDir:	DEVICE OSCHS3
	IRC_CAL IRC_FAST
                        ;#pragma asmDir:	IRC_CAL IRC_FAST
	FREQ	50_000_000
                        ;#pragma asmDir:	FREQ	50_000_000
	ID	'project'
                        ;#pragma asmDir:	ID	'project'
                        ;
                        ;// ************************************************
                        ;// ************************************************
                        ;// MACRO DEFINITIONS AND CONSTANTS
                        ;
                        ;#define MAX_COUNT1 10
                        ;#define MAX_COUNT2 255
                        ;#define MAX_COUNT3 255
                        ; 
                        ;// ************************************************
                        ;// ************************************************
                        ;// IO PORT DEFINITIONS
                        ;
                        ;bit pinB0 @ RB.0;
                        ;
                        ;// ************************************************
                        ;// ************************************************
                        ;// GLOBAL VARIABLE DEFINITIONS
                        ;
                        ;#pragma rambank 0
                        ;// unsigned char Count1, Count2;          // located in ram bank 0
                        ;
                        ;#pragma rambank 2
                        ;
                        ;
                        ;// ************************************************
                        ;// ************************************************
                        ;// PROTOTYPES AND CODEPAGES
                        ;
                        ;/* Prototypes are needed when a function is called before it
                        ;is defined. It can also be used for fine tuning the code
                        ;layout (i.e. which codepage to use for a function). */
                        ;
                        ;void delay( void);
                        ;page2 char function2( void);
                        ;
                        ;
                        ;
                        ;// ************************************************
                        ;// ************************************************
                        ;// INTERRUPT SUPPORT
                        ;
                        ;#pragma origin 0   // interrupt must be located first on codepage 0
        ORG $0000
                        ;
                        ;interrupt iServer(void)
                        ;{
iServer
                        ;    /* NOTE: W, STATUS and FSR are automatically saved to
                        ;       and restored from shadow registers */
                        ;
                        ;    nop();
        NOP  
                        ;}
        RETI 
                        ;
                        ;
                        ;
                        ;// ************************************************
                        ;// ************************************************
                        ;// INCLUDED C FILES AND MATH ROUTINES
                        ;
                        ;// none in this program
                        ;
                        ;#pragma codepage 0
                        ;
                        ;// ************************************************
                        ;// ************************************************
                        ;// C FUNCTIONS
                        ;
                        ;
                        ;void delay( void)
                        ;{
delay
                        ;	unsigned char Count1, Count2, Count3;  
                        ;	for (Count1 = MAX_COUNT1; Count1 != 0; Count1--) {
        MOV   W,#10
        BANK  64
        MOV   Count1,W
m001    BANK  64
        TEST  Count1
        SNB   3.Zero_
        JMP   m006
                        ;		for (Count2 = MAX_COUNT2; Count2 != 0; Count2--) {
        MOV   W,#255
        MOV   Count2,W
m002    BANK  64
        TEST  Count2
        SNB   3.Zero_
        JMP   m005
                        ;			for (Count3 = MAX_COUNT3; Count3 != 0; Count3--) {
        MOV   W,#255
        MOV   Count3,W
m003    BANK  64
        TEST  Count3
        SNB   3.Zero_
        JMP   m004
                        ;			// do nothing
                        ;			}
        DEC   Count3
        JMP   m003
                        ;		}
m004    BANK  64
        DEC   Count2
        JMP   m002
                        ;	}
m005    BANK  64
        DEC   Count1
        JMP   m001
                        ;	
                        ;	return;
m006    RET  
                        ;
                        ;}
                        ;
                        ;
                        ;void main(void)
                        ;{
main
                        ;
                        ;	// OPTION = 0xa8;  // ints off!
                        ;	
                        ;    clearRAM();  // built in function
        MOV   W,#8
        MOV   FSR,W
m007    CLR   INDF
        INC   FSR
        SNB   FSR.4
        JMP   m007
        SNB   FSR.3
        JMP   m007
        MOV   W,#16
        ADD   FSR,W
        XOR   W,FSR
        SB    3.Zero_
        JMP   m007
                        ;#if 0
                        ;	
                        ;	    // configure 8-bit RB, all outputs
                        ;	TRISB = 0x00;   // all pins output
                        ;	LVL_B = 0xff;   // all TTL voltages
                        ;    ST_B = 0xff;    // schmitt triggers off
                        ;    PLP_B = 0xff;   // pull-ups off
                        ;    WKEN_B = 0xff;  // Wake up disabled
                        ;    WKED_B = 0xff;  // Wake up falling edge
                        ;
                        ;    // configure 8-bit RC, all outputs
                        ;	TRISC = 0x00;   // all pins output
                        ;	LVL_C = 0xff;   // all TTL voltages
                        ;    ST_C = 0xff;    // schmitt triggers off
                        ;    PLP_C = 0xff;   // pull-ups off
                        ;
                        ;    RB = 0xff;      // all LEDs off!
                        ;	
                        ;#endif
                        ;	
                        ;	bank0 unsigned char hold;
                        ;	bit holdB0 @ hold.0;	
                        ;	// bank1 char c;
                        ;
                        ;	TRISB = 0xfe;   // %1111 1110
        MOV   M,#15
        MOV   W,#254
        MOV   !RB,W
                        ;	RB = 0;
        CLR   RB
                        ;	while (1) {
                        ;	  delay();
m008    CALL  delay
                        ;	  holdB0 = pinB0;
        BANK  0
        CLRB  16.holdB0
        SNB   6.pinB0
        SETB  16.holdB0
                        ;	  hold++;
        INC   hold
                        ;	  pinB0 = holdB0;
        SB    16.holdB0
        CLRB  6.pinB0
        SNB   16.holdB0
        SETB  6.pinB0
                        ;	}
        JMP   m008

        RESET main
        END


; *** KEY INFO ***

; 0x0002 P0   28 word(s)  5 % : delay
; 0x0000 P0    2 word(s)  0 % : iServer
; 0x001E P0   28 word(s)  5 % : main

; RAM usage: 4 bytes (4 local), 132 bytes free
; Maximum call level: 1 (+1 for interrupt)
;  Codepage 0 has   58 word(s) :  11 %
;  Codepage 1 has    0 word(s) :   0 %
;  Codepage 2 has    0 word(s) :   0 %
;  Codepage 3 has    1 word(s) :   0 %
; Total of 59 code words (2 %)
