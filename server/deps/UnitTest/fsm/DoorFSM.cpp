#include "DoorFSM.h"

void DoorFSM::__processEvent( Event e )
{
   States yOld = __Y;
   bool pass = false;
   switch( __Y ) { //transitions 
   case Closed:
      if( e == Open ) {
         //outcome actions
         __Y = Opened;
         pass = true;
      }
      else if( e == Lock ) {
         //outcome actions
         __Y = Locked;
         pass = true;
      }
      break;
   case Unlocked:
      if( e == Lock ) {
         //outcome actions
         __Y = Locked;
         pass = true;
      }
      else if( e == Open ) {
         //outcome actions
         __Y = Opened;
         pass = true;
      }
      break;
   case Locked:
      if( e == Unlock ) {
         //outcome actions
         __Y = Unlocked;
         pass = true;
      }
      break;
   case Opened:
      if( e == Close ) {
         //outcome actions
         __Y = Closed;
         pass = true;
      }
      break;
   }

   if( yOld == __Y && !pass ) { return; }

   switch( __Y ) { // income actions
   case Closed:
         enterClosed();
      break;
   case Unlocked:
         enterUnlocked();
      break;
   case Locked:
         enterLocked();
      break;
   case Opened:
         enterOpened();
      break;
   }
}

void DoorFSM::A( Event e )
{
   bool __empty = __events.empty();
   __events.push( e );
   if( __empty ) {
      while( !__events.empty() ) {
         __processEvent( __events.front() );
         __events.pop();
      }
   }
}

const char* DoorFSM::eventName( Event e )
{
   switch(e) {
   case Lock:
      return "Lock";
   case Unlock:
      return "Unlock";
   case Open:
      return "Open";
   case Close:
      return "Close";
   default:
      return "Unknown event";
   }
}

const char* DoorFSM::stateName( States s )
{
   switch(s) {
   case Closed:
      return "Closed";
   case Unlocked:
      return "Unlocked";
   case Locked:
      return "Locked";
   case Opened:
      return "Opened";
   default:
      return "Unknown state";
   }
}
