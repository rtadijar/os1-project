// File: event.h
#ifndef _event_h_
#define _event_h_


#include "IVTEntry.h"
#include "Kernel.h"


typedef unsigned char IVTNo;
class KernelEv;


class Event {


public:
	Event (IVTNo ivtNo);
	~Event ();

	void wait  ();

protected:
   friend class KernelEv;
   void signal(); // can call KernelEv

private:

   KernelEv* myImpl;

};

#define PREPAREENTRY(entry, callOld) \
void interrupt interrupt_##entry(...); \
IVTEntry entry_##entry(entry, interrupt_##entry); \
void interrupt interrupt_##entry(...) { \
	entry_##entry.signal(); \
	if (callOld) (*entry_##entry.oldRoutine)(); \
	if (!Kernel::locked()) dispatch(); \
}\

#endif
