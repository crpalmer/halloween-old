
// -----------------------------------------------------------
// MAILBOX LIBRARY FOR LEANSLICE MULTITASKING
//
//  Copyright (c) B Knudsen Data, Trondheim, Norway, 1999-2002
// -----------------------------------------------------------

/*
  Mailboxes are intended for exchanging byte messages between tasks.
  The available mailbox interface allows various degrees of
  synchronization between the tasks. Note that a mailbox can only
  contain a single message. There is no queue for storing multiple
  messages waiting to be processed. However, the sender can check if
  the receiver have removed the message from the mailbox, and
  receive acknowledge message.

  Required definitions in the application program:

   enum {MboxTask1, MB2};  // Mailbox identifiers
   #define NoOfMailBoxes 2
   #define MSG_TYPE      1
   #include "msg.h"

  Initialization:

    initMbox(MboxTask1);    // initialize in main()

  Interface:

   putMsg(MboxNo, Msg)      // write message, overwrite previous
   waitUntilMsgRead(MboxNo) // sender waits until message is removed
   deliverMsg(MboxNo, Msg)  // write message, then wait until removed
   waitUntilMsgAck(MboxNo)  // wait for acknowledge (type 1,3,4)

   getMsgCopy(MboxNo)  // read message without removing it
   hasMsg(MboxNo)      // test if mailbox contain a message
   noMsg(MboxNo)       // test if mailbox is empty
   clearMsg(MboxNo)    // delete message 
   ackOnMsg(MboxNo)    // test if acknowledge on message (type 1,3,4)
   setAckOnMsg(MboxNo) // set acknowledge on message (type 1,3,4)

   getMsg(MboxNo, mVar) // read message and remove it (acknowledge)
   getNextMsg(MboxNo, mVar) // wait for message and read (remove) it
   waitForMsg(MboxNo)   // wait until message arrives

  Using mailboxes for exchanging byte messages:

   // sending task
   putMsg(MB2, 9);         // write message 9 to mailbox MB2
   waitUntilMsgRead(MB2);  // wait for receiver to remove the message

   // receiving task
   char mm;
   waitForMsg(MB2);  // wait until message arrives
   getMsg(MB2, mm);  // read message to mm and clear mailbox

 Message interpretation:

 Type 1: Return messages and empty mailbox share representation (most compact)
   0-0x7F : messages
   0x80-0xFF: empty mailbox or acknowledge messages

 Type 2: No return messages
   0 : empty mailbox (mail removed)
   1-0xFF: messages

 Type 3: Includes return messages and specific empty mailbox representation
  0 : empty mailbox (mail removed)
  1-0x7F: messages
  0x80-0xFF: return/acknowledge messages

 Type 4: Return messages and empty mailbox share representation
   0-0x7F : messages
   0x80-0xFF: empty mailbox or acknowledge messages
   - can only be used together with type 3 tasks
   - receiver is suspended until message is ready
   - ADVANTAGE: faster execution because receiver is suspended
   - DISADVANTAGE: requires more code and more RAM
   - NOTE: task state addresses and mailbox addresses have to reside
     in RAM below address 0x100 in this implementation
*/


#ifndef MSG_TYPE
 #error The MSG_TYPE have to be defined (1, 2, 3, 4)
#endif


// ----------------- TYPE 1 ----------------------
#if MSG_TYPE == 1

char Mbox[NoOfMailBoxes];

#define initMbox(MboxNo)  Mbox[MboxNo].7 = 1

#define putMsg(MboxNo, Msg)  Mbox[MboxNo] = Msg

#define waitUntilMsgRead(MboxNo)  while (!Mbox[MboxNo].7) waitState()

#define deliverMsg(MboxNo, Msg) \
{ \
    Mbox[MboxNo] = Msg; \
    while (!Mbox[MboxNo].7) waitState(); \
}

#define waitUntilMsgAck(MboxNo)  while (!Mbox[MboxNo].7) waitState()

#define getMsgCopy(MboxNo)  Mbox[MboxNo]
#define hasMsg(MboxNo)  !Mbox[MboxNo].7
#define noMsg(MboxNo)  Mbox[MboxNo].7
#define clearMsg(MboxNo)  Mbox[MboxNo].7 = 1

#define ackOnMsg(MboxNo)  Mbox[MboxNo].7

#define setAckOnMsg(MboxNo)  Mbox[MboxNo].7 = 1

#define getMsg(MboxNo, mVar) \
{ \
    mVar = Mbox[MboxNo]; \
    Mbox[MboxNo].7 = 1; \
}

#define getNextMsg(MboxNo, mVar) \
{ \
    while (Mbox[MboxNo].7) \
        waitState(); \
    mVar = Mbox[MboxNo]; \
    Mbox[MboxNo].7 = 1; \
}

#define waitForMsg(MboxNo) \
{ \
    while (Mbox[MboxNo].7) \
        waitState(); \
}





// ----------------- TYPE 2 ----------------------
#elif MSG_TYPE == 2
char Mbox[NoOfMailBoxes];

#define initMbox(MboxNo)  Mbox[MboxNo] = 0

#define putMsg(MboxNo, Msg)  Mbox[MboxNo] = Msg

#define waitUntilMsgRead(MboxNo)  while (Mbox[MboxNo]) waitState()

#define deliverMsg(MboxNo, Msg) \
{ \
    Mbox[MboxNo] = Msg; \
    while (Mbox[MboxNo]) waitState(); \
}

// NOT AVAILABLE: waitUntilMsgAck(MboxNo)

#define getMsgCopy(MboxNo)  Mbox[MboxNo]
#define hasMsg(MboxNo)  Mbox[MboxNo]
#define noMsg(MboxNo)  !Mbox[MboxNo]
#define clearMsg(MboxNo)  Mbox[MboxNo] = 0

// NOT AVAILABLE: ackOnMsg(MboxNo)

// NOT AVAILABLE: setAckOnMsg(MboxNo)

#define getMsg(MboxNo, mVar) \
{ \
    mVar = Mbox[MboxNo]; \
    Mbox[MboxNo] = 0; \
}

#define getNextMsg(MboxNo, mVar) \
{ \
    while (Mbox[MboxNo] == 0) \
        waitState(); \
    mVar = Mbox[MboxNo]; \
    Mbox[MboxNo] = 0; \
}

#define waitForMsg(MboxNo) \
{ \
    while (Mbox[MboxNo] == 0) \
        waitState(); \
}




// ----------------- TYPE 3 ----------------------
#elif MSG_TYPE == 3

char Mbox[NoOfMailBoxes];

#define initMbox(MboxNo)  Mbox[MboxNo] = 0

#define putMsg(MboxNo, Msg)  Mbox[MboxNo] = Msg

#define waitUntilMsgRead(MboxNo) \
    while (Mbox[MboxNo]  &&  !Mbox[MboxNo].7) waitState()

#define deliverMsg(MboxNo, Msg) \
{ \
    Mbox[MboxNo] = Msg; \
    while (Mbox[MboxNo]  &&  !Mbox[MboxNo].7) waitState(); \
}

#define waitUntilMsgAck(MboxNo)  while (!Mbox[MboxNo].7) waitState()

#define getMsgCopy(MboxNo)  Mbox[MboxNo]
#define hasMsg(MboxNo)  (Mbox[MboxNo] && !Mbox[MboxNo].7)
#define noMsg(MboxNo)  (!Mbox[MboxNo] || Mbox[MboxNo].7)
#define clearMsg(MboxNo)  Mbox[MboxNo] = 0

#define ackOnMsg(MboxNo)  Mbox[MboxNo].7

#define setAckOnMsg(MboxNo)  Mbox[MboxNo].7 = 1

#define getMsg(MboxNo, mVar) \
{ \
    mVar = Mbox[MboxNo]; \
    Mbox[MboxNo] = 0; \
}

#define getNextMsg(MboxNo, mVar) \
{ \
    while (!Mbox[MboxNo]  ||  Mbox[MboxNo].7) \
        waitState(); \
    mVar = Mbox[MboxNo]; \
    Mbox[MboxNo] = 0; \
}

#define waitForMsg(MboxNo) \
{ \
    while (!Mbox[MboxNo]  ||  Mbox[MboxNo].7) \
        waitState(); \
}




// ----------------- TYPE 4 ----------------------
#elif MSG_TYPE == 4

struct  {
   char *receiver;
   char msg;
} Mbox[NoOfMailBoxes];

#define initMbox(MboxNo)  Mbox[MboxNo].msg.7 = 1

#define putMsg(MboxNo, Msg) \
{ \
    Mbox[MboxNo].msg = Msg; \
    if (Mbox[MboxNo].receiver) { \
        *Mbox[MboxNo].receiver &= 0x7F; \
        Mbox[MboxNo].receiver = 0; \
    } \
}

#define waitUntilMsgRead(MboxNo)  while (!Mbox[MboxNo].msg.7) waitState()

#define deliverMsg(MboxNo, Msg) \
{ \
    Mbox[MboxNo].msg = Msg; \
    if (Mbox[MboxNo].receiver) { \
        *Mbox[MboxNo].receiver &= 0x7F; \
        Mbox[MboxNo].receiver = 0; \
    } \
    while (!Mbox[MboxNo].msg.7) waitState(); \
}

#define waitUntilMsgAck(MboxNo)  while (!Mbox[MboxNo].msg.7) waitState()

#define getMsgCopy(MboxNo)  Mbox[MboxNo].msg
#define hasMsg(MboxNo)  !Mbox[MboxNo].msg.7
#define noMsg(MboxNo)  Mbox[MboxNo].msg.7
#define clearMsg(MboxNo)  Mbox[MboxNo].msg.7 = 1

#define ackOnMsg(MboxNo)  Mbox[MboxNo].msg.7

#define setAckOnMsg(MboxNo)  Mbox[MboxNo].msg.7 = 1

#define getMsg(MboxNo, mVar) \
{ \
    mVar = Mbox[MboxNo].msg; \
    Mbox[MboxNo].msg.7 = 1; \
}

#define getNextMsg(MboxNo, mVar) \
{ \
    if (Mbox[MboxNo].msg.7)  { \
        Mbox[MboxNo].receiver = getStateAddress(); \
        waitUntilResume(); \
    } \
    mVar = Mbox[MboxNo].msg; \
    Mbox[MboxNo].msg.7 = 1; \
}


#define waitForMsg(MboxNo) \
{ \
    if (Mbox[MboxNo].msg.7)  { \
        Mbox[MboxNo].receiver = getStateAddress(); \
        waitUntilResume(); \
    } \
}

#endif

