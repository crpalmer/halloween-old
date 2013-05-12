/*
  ----------------------------------------------------------
  SEMAPHORE LIBRARY FOR LEANSLICE MULTITASKING

  Copyright (c) B Knudsen Data, Trondheim, Norway, 1999-2002
  ----------------------------------------------------------

  ==> Semaphores are frequently used for protecting critical common
      data, regions or application resources. The value of  the
      implemented semaphores can range from 0 to 255, and is also
      called a counting semaphore. A granted semaphore count
      normally represent a permission to use a shared resource. The
      actual interpretation depends on the application. Note that
      binary semaphores normally satisfy simple needs to protect
      data. However, if many tasks are allowed to read or write
      shared data simultaneously, then a counting semaphore is worth
      considering.

  Required definitions in the application program:

   enum {Semaphore1, SM2};  // Semaphore identifiers
   #define NoOfSemaphores 2
   #define SEM_TYPE       1
   #include "semaphore.h"

  Initialization:

   initSemaphore(SM2,3);   // initialize semaphore (in main())

  Using semaphores:

   getSemaphore(SM2,1);    // (wait) and get semaphore
   //.. process protected region/data/resource
   freeSemaphore(SM2,1);   // free semaphore (on exit)


  Semaphore types:

  Type 1: blocking implementation: the highest semaphore request
   blocks other requests. Example: a semaphore is given the value 5.
   Up to 5 tasks hunts for semaphore value 1 each to enter a
   critical region. A single task needs to update the data in the
   region and asks for all 5 semaphore counts. The reading tasks are
   then blocked until the writing task are granted all 5 semaphore
   counts and have returned them. The blocking approach simulates a
   queue with 2 levels.

  Type 2: non-blocking implementation: free semaphores are always
   granted. Tasks requesting for many semaphore counts never blocks
   tasks requesting for an available number of semaphore counts.
   Tasks requesting many semaphore counts may thus have to wait
   long.

  Type 3: non-blocking implementation, same as type 2. Slower
   execution, but possibly less code when semaphore identifiers are
   allocated dynamically in the application (i.e. the semaphore
   identifier is stored in a variable instead of being represented
   by a fixed constant).
*/


#ifndef SEM_TYPE
 #error The SEM_TYPE have to be defined (1,..)
#endif


// -------------------------------------------
#if SEM_TYPE == 1

 #if NoOfSemaphores == 1

  struct  {
     char noOf;
     char request;
  } Semaphore;

  #define initSemaphore(SemNo,Value)  \
  { \
      Semaphore.noOf = Value; \
      Semaphore.request = 0; \
  }

  #define freeSemaphore(SemNo,Value)  Semaphore.noOf += Value

  #define getSemaphore(SemNo,Value) \
     while (!getSemaphore__(Value)) \
         waitState()

  bit getSemaphore__(char Value)
  {
    /*
      if (Semaphore.noOf >= Value  &&  Semaphore.request < Value)  {
          Semaphore.noOf -= Value;
          Semaphore.request = 0;
          return 1;
      }
      if (Semaphore.request < Value)
          Semaphore.request = Value;
      return 0;
    */
      W = Semaphore.request - Value;
      if (Carry == 1)
          return 0;
      Semaphore.request = W;
      W = Semaphore.noOf - Value;
      if (Carry == 0)
          return Carry;
      Semaphore.noOf = W;
      Semaphore.request = 0;
      return Carry;
  }

 #else

  struct  {
     char noOf;
     char request;
  } Semaphore[NoOfSemaphores];

  #define initSemaphore(SemNo,Value)  \
  { \
      Semaphore[SemNo].noOf = Value; \
      Semaphore[SemNo].request = 0; \
  }

  #define freeSemaphore(SemNo, Value)  Semaphore[SemNo].noOf += Value

  #define getSemaphore(SemNo,Value) \
     while (!getSemaphore__(SemNo,Value)) \
         waitState()

  bit getSemaphore__(char SemNo, char Value)
  {
      FSR = &Semaphore[SemNo].request;
     #if __IRP_RAM__
      IRP = &Semaphore[0].request / 256;
     #endif

      W = INDF - Value;
      if (Carry == 1)
          return 0;
      INDF = W;
      FSR --;
      W = INDF - Value;
      if (Carry == 0)
          return Carry;
      INDF = W;
      FSR ++;
      INDF = 0;
      return Carry;
  }

 #endif


// -------------------------------------------
#elif SEM_TYPE == 2

 struct  {
    char noOf;
 } Semaphore[NoOfSemaphores];

 #define initSemaphore(SemNo,Value)  Semaphore[SemNo].noOf = Value

 #define freeSemaphore(SemNo, Value)  Semaphore[SemNo].noOf += Value

 #define getSemaphore(SemNo,Value) \
 { \
     while (1)  { \
         W = Semaphore[SemNo].noOf - Value; \
         if (Carry) \
             break; \
         waitState(); \
     } \
     Semaphore[SemNo].noOf = W; \
 }


// -------------------------------------------
#elif SEM_TYPE == 3

 #if NoOfSemaphores == 1

  struct  {
     char noOf;
  } Semaphore;

  #define initSemaphore(SemNo,Value)  Semaphore.noOf = Value

  #define freeSemaphore(SemNo,Value)  Semaphore.noOf += Value

  #define getSemaphore(SemNo,Value) \
  { \
     while (!getSemaphore__(Value)) \
         waitState(); \
  }

  bit getSemaphore__(char W)
  {
      W = Semaphore.noOf - W;
      if (Carry == 0)
          return Carry;
      Semaphore.noOf = W;
      return Carry;
  }

 #else

  struct  {
     char noOf;
  } Semaphore[NoOfSemaphores];

  #define initSemaphore(SemNo,Value)  Semaphore[SemNo].noOf = Value

  #define freeSemaphore(SemNo, Value)  Semaphore[SemNo].noOf += Value

  #define getSemaphore(SemNo,Value) \
     while (!getSemaphore__(SemNo,Value)) \
         waitState()

  bit getSemaphore__(char SemNo, char Value)
  {
      FSR = &Semaphore[SemNo];

      W = INDF - Value;
      if (Carry == 0)
          return Carry;
      INDF = W;
      return Carry;
  }

 #endif

#endif
