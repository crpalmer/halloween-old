/*
  ----------------------------------------------------------
  EVENT LIBRARY FOR LEANSLICE MULTITASKING

  Copyright (c) B Knudsen Data, Trondheim, Norway, 1999-2002
  ----------------------------------------------------------

  ==> Events can be used to broadcast information between tasks. The
      implemented event type is simple and each event maps to a
      single bit. Tasks waiting for an event will use active
      waiting. When using round robin scheduling, it is enough to
      set the event to 1 and then do an task switch. This ensures
      that the tasks waiting for the event will detect it. The event
      can be CLEARED at the next scheduling of the task that set it
      to true. It is of course possible to use the event mechanism
      in other ways also.

  Required definitions in the application program:

   enum {EventA, EventB};  // Event identifiers
   #define NoOfEvents     2
   #define EVENT_TYPE     1
   #include "event.h"

  Initialization:

   initEvents();   // initialization in main()

  Using events:

   setEvent(EventA);  // set event to TRUE
   waitState();        // enable waiting tasks to detect the event
   // the event can be set to false immediate or delayed
   clearEvent(EventA); // set event to FALSE

   waitUntilEvent(EventA);  // task waiting for event, active waiting

  Event Types:

  Type 1:  8 - 32 events, with start and stop
   - compact and efficient implementation (active waiting only)
*/

#ifndef EVENT_TYPE
 #error The EVENT_TYPE have to be defined (1,..)
#endif



// -------------------------------------------
#if EVENT_TYPE == 1

#if NoOfEvents <= 8
 char EventStates;
#elif NoOfEvents <= 16
 uns16 EventStates;
#elif NoOfEvents <= 24
 uns24 EventStates;
#else
 uns32 EventStates;
#endif

#define initEvents()  EventStates = 0

#define setEvent(EventNo)  EventStates.EventNo = 1
#define clearEvent(EventNo)  EventStates.EventNo = 0

#define waitUntilEvent(EventNo) while (!EventStates.EventNo) waitState()


#endif
