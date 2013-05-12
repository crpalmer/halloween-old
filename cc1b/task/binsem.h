/*
  ----------------------------------------------------------
  BINARY SEMAPHORE LIBRARY FOR LEANSLICE MULTITASKING

  Copyright (c) B Knudsen Data, Trondheim, Norway, 1999-2002
  ----------------------------------------------------------

  ==> Binary semaphores are used for protecting critical common data
      or regions. This ensures that only one task is granted access
      at the same time.

  Required definitions in the application program:

   enum {BinSemaphore1, BS2};
   #define NoOfBinSemaphores 2
   #define BINSEM_TYPE       1
   #include "binsem.h"

  Initialization:

   initBinSemaphores();  // initialize in main()

  Using binary semaphores:

   getBinSemaphore(BS2);  // get semaphore (wait until free)
   //.. process protected region
   freeBinSemaphore(BS2);  // release semaphore

  The single binary semaphore type available allows up
  to 8, 16, 24 or 32 binary semaphores.
*/

#ifndef BINSEM_TYPE
 #error The BINSEM_TYPE have to be defined (1,..)
#endif


// -------------------------------------------
#if BINSEM_TYPE == 1

#if NoOfBinSemaphores <= 8
 char BinSemaphore;
#elif NoOfBinSemaphores <= 16
 uns16 BinSemaphore;
#elif NoOfBinSemaphores <= 24
 uns24 BinSemaphore;
#else
 uns32 BinSemaphore;
#endif

#define initBinSemaphores()  BinSemaphore = 0

#define freeBinSemaphore(BSemNo)  BinSemaphore.BSemNo = 0

#define getBinSemaphore(BSemNo) \
{ \
   while (BinSemaphore.BSemNo == 1) \
       waitState(); \
   BinSemaphore.BSemNo = 1; \
}


#endif
