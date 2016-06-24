#ifndef DoorFSM629564419
#define DoorFSM629564419
#include <queue> // for event queue

class DoorFSM {
public:
/** Events*/
   enum Event { Lock, Unlock, Open, Close   };
protected:


   virtual void enterOpened() = 0; 
   virtual void enterLocked() = 0; 
   virtual void enterUnlocked() = 0; 
   virtual void enterClosed() = 0; 

public:
  /** States */
   enum States { Closed, Unlocked, Locked, Opened   }; // states

private:
   States __Y;
   std::queue<Event> __events; 
   void __processEvent( Event e );
public:
   /// Constructor
   DoorFSM() { __Y = Opened; }
   /// Destructor
   virtual ~DoorFSM() {}
   /** Get current FSM state
       @returns current FSM state
    */
   States currentState() { return __Y; }


   /** Get symbolic name of an event

      @param e - numeric event
      @returns symbolic event name
    */
   static const char* eventName( Event e );


   /** Get symbolic name of a state

      @param e - numeric state
      @returns symbolic state name
    */
   
   static const char* stateName( States s );


   /** Send event to FSM

       Use this function to send event to DoorFSM
       After you call it given event will be handled,
       and, if some of transition conditions match, 
       appropriate transition will be triggered,
       and currentState() will be changed. 

       If this function is called during existing event
       handling process, given event will be added to 
       pending event queue, and will be handled after current
       transition. See examples for details.
   */
   void A( Event e );
};

#endif
