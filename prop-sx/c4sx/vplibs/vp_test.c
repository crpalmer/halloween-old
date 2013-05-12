// File vp_test.c --test program for CC1B compiler

#pragma chip SX48 // select device
//---------------------------------------------------------------------------------
#if defined _CHIP_SX18_ || defined _CHIP_SX20_ || defined _CHIP_SX28_
#pragma asmDir:	DEVICE	OSCHS1, TURBO, STACKX, OPTIONX ;SX18/20/28
#endif
#if defined _CHIP_SX48_ || defined _CHIP_SX52_
#pragma asmDir:	DEVICE	OSCHS1 ;SX48/52
#endif
#pragma asmDir:	IRC_CAL IRC_SLOW
#pragma asmDir:	FREQ	20_000_000
#pragma asmDir:	RESET	main
#pragma asmDir:	ID	'project'
//======================================================

#define CPUFREQ 20000000
#define MAXBAUD 57600
//#define ISRTHREADS 4
#include <vpobjDefs.h>

//#pragma mainStack 0 @0xF0	//allocate locals from ram address 0xF0 upwards (use only when <= 16 locals)

// ************************************************
// ************************************************
// MACRO DEFINITIONS AND CONSTANTS

// Area for storage of constant data (at end of program code)
// The size of the area is set by the constant after _CHIP_CODESIZE_
// The minimal constant value is 1. Set it to the minimal value as required by the application.
#pragma cdata[_CHIP_CODESIZE_ - 0x100]

// ************************************************
// ************************************************
// IO PORT DEFINITIONS

bit pinTX @RA.1;
bit pinRX @RA.2;

/*
 * CONNECTION diagram
 *
 *                                    | +5V
 *                                  /
 *  RA.1 -----[4k7]-----|  PNP
 *                                  \
 *                                   +----- SOUT    ------------->    To PC comport receive
 *                               [4k7]
 *                                   |
 *  RA.2 -----[33k]------+----- SIN    ---------------<    From PC comport transmit
 */

// ************************************************
// ************************************************
// GLOBAL VARIABLE DEFINITIONS

#pragma membank0			//allocate globals from global ram
//define variables that must reside in global ram here

#pragma membank1			//allocate globals from ram address 0x10 upwards

// ************************************************
// ************************************************
// PROTOTYPES AND CODEPAGES
/* Prototypes are needed when a function is called before it
is defined. It can also be used for fine tuning the code
layout (i.e. which codepage to use for a function). */

// ************************************************
// ************************************************
// INCLUDED C FILES AND MATH ROUTINES
#pragma codepage 1
#include <memory.h>
#include <Portpin.h>
#pragma membank6

#pragma codepage 2
#include <vpUart.h>
#pragma membank9
#pragma codepage 3
#include <objQueue.h>
#pragma membank11

// ************************************************
// ************************************************
// VIRTUAL PERIPHERAL AND OBJECT DECLARATIONS

#pragma cdata.VPDEFINITIONS //required label to identify start of VP definitions
vpUartTx(TX1,PORTPIN_RA1,HIGHOUTPUT_HIGHOUTPUT,0,0,9600,1,E72)
vpUartRx(RX1,PORTPIN_RA2+PORTPIN_INV,LOWINPUT_LOWINPUT,0,0,9600,1,E72)

#define  VPSLOTS  2     //set number of VP slots (SX18/20/28: 1 to 6, SX48/52: 1 to 8)
#define  VPRAM    0x30  //set first vp ram bank (total banks allocated is VPSLOTS+1)
#include <vpobjLib.h>
#pragma codepage 4

#define TASKS 2        //number of tasks that can run concurrently
#define TASKTICK 60    //number of ISR ticks that make up a tasktick
#define TASKLIST 0x70  //locates TaskList at 0x70
#include <Task.h>

// ************************************************
// ************************************************
// INTERRUPT SUPPORT

//include global variables for interrupt here
#pragma unlockISR
#pragma origin 0   // without #pragma unlockISR, interrupt must be located first on codepage 0
#pragma asmDir:	JMP	@iServer
#pragma asmDir:	JMP	@TaskSchedule
#pragma origin 4
interrupt iServer(void) {
  /* NOTE: W, STATUS and FSR are automatically saved to and restored from shadow registers */
#if defined _CHIP_SX18_ || defined _CHIP_SX20_ || defined _CHIP_SX28_
  _IsrMode = MODE; //save M
#endif
  char i,vp;
  _IsrBank = VPRAM;
  for (i=0; i<VPSLOTS; i++) {
    vp = vpListType[i];
    switch (vp) {
#ifdef vpTimer16_installed
      case VP_TIMER16: vpTimer16_ISR;
                       break;
#endif
#ifdef vpUartRx_installed
      case VP_UARTRX: vpUartRx_ISR;
                      break;
#endif
#ifdef vpUartTx_installed
      case VP_UARTTX: vpUartTx_ISR;
                      break;
#endif
    }
    W = 0x10;
    _IsrBank += W;
#if defined _CHIP_SX18_ || defined _CHIP_SX20_ || defined _CHIP_SX28_
    _IsrBank += W;
#endif
  }
  Task_ISR; //interrupt part of the task scheduler
#if defined _CHIP_SX18_ || defined _CHIP_SX20_ || defined _CHIP_SX28_
  MODE = _IsrMode; //restore M
#endif
  W = -INTPERIOD;
  retiw();
}

// ************************************************
// ************************************************
// TASK FUNCTIONS

#pragma membank9
char tx,rx; //handles for uart transmit and uart receive

TASK RxEnqueue() {
  //store received byte in receiver queue
  //if queue is full, byte is not stored
  if (vpUartRx_byteAvailable(rx)) {
    if (!objQueue_isFull(vpUart_queue(rx))) {
      _TaskTemp = vpUartRx_receiveByte(rx);
	  objQueue_enqueue(vpUart_queue(rx),_TaskTemp);
    }
  }
}

TASK TxDequeue() {
  //if transmitter ready to transmit and transmitter queue not empty, then
  //retrieve byte from transmitter queue and send it to transmitter
  if (vpUartTx_ready(tx)) {
    if (!objQueue_isEmpty(vpUart_queue(tx))) {
      _TaskTemp = objQueue_dequeue(vpUart_queue(tx));
	  vpUartTx_sendByte(tx,_TaskTemp);
    }
  }
}

#define TXDEQUEUE 0 //identifier definitions for task subroutines
#define RXENQUEUE 1

void TaskSwitch(char W) {
  switch (W & 0x3F) { //call task, returns within 1 tasktick
	  case TXDEQUEUE: TxDequeue(); break; //TxDequeue has identifier 0
	  case RXENQUEUE: RxEnqueue(); break; //RxEnqueue has identifier 1
  }
}

// ************************************************
// ************************************************
// C FUNCTIONS

unsigned long wresult;

char enqueue(char value) {
  //store byte in transmitter queue
  //returns 0 if byte not stored
  //returns 1 if byte stored
  if (!objQueue_isFull(vpUart_queue(tx))) {
	_MainTemp = value;
    objQueue_enqueue(vpUart_queue(tx),_MainTemp);
    return 1;
  }
  return 0;
}

void sendChar(char value) {
  //transmit a char
  //this sub blocks until char transmitted
  char result;
  do {
    result = enqueue(value);
  } while (result == 0);
}

void sendString(long addr) {
  //transmit a string
  //this sub blocks until string transmitted
  char value;
  do {
    value = RomChar(addr);
    if (value == 0) break;
    sendChar(value);
    addr++;
  } while (1);
}

long dequeue() {
  //retrieve byte from receiver queue
  //if queue is empty, -1 is returned
  char value;
  if (!objQueue_isEmpty(vpUart_queue(rx))) {
    value = objQueue_dequeue(vpUart_queue(rx));
    return value;
  }
  return -1;
}

#pragma cdata.WELCOME = "Welcome to the world of taskswitching C",13,0

void main() {
  clearRAM(); //clear all ram
  DDR_init(); //intialize DDR copy registers
  //initialize and install VP's
  vpInit(); //must be called prior to other vp functions
  tx = vpInstall(TX1); //install virtual peripheral TX1
  rx = vpInstall(RX1); //install virtual peripheral RX1
  //setup tasks
  TaskInit(); //must be called prior to other task functions
  TaskSet(0,TXDEQUEUE+TASKSTART,2); // task slot 0, run "TxDequeue", every 2 taskticks
  TaskSet(1,RXENQUEUE+TASKSTART,2); // task slot 1, run "RxEnqueue", every 2 taskticks
  TaskEnable(); //master task enable (eg. enable task interrupts)
  //at this point, the interrupt should be enabled
  OPTION = RTCC_ON + RTCC_PS_OFF;
  sendString(WELCOME);
  while (1) {
    wresult = dequeue(); // get received byte (from receiver queue)
    if (wresult.high8 == 0) { //queue not empty, so result valid
      sendChar(wresult.low8); //echo received byte
    }
  }
}

// End of file vp_test.c
