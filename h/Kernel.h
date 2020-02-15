/*
 * Kernel.h
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#define lock Kernel::lockCnt++;
#define unlock if(Kernel::lockCnt != 0) Kernel::lockCnt--; if(Kernel::lockCnt == 0 && Kernel::contextChangeRequested) dispatch();

#define hard_lock asm {pushf; cli;};
#define hard_unlock asm {popf;};

class Idle;
class Semaphore;

class Kernel {
public:
	static void initOS();
	static void restoreOS();

	static int locked();
private:

	friend class KernelSem;
	friend class PCB;
	friend class Thread;
	friend class SleepingList;
	friend class ThreadList;
	friend class HandlerList;
	friend class SignalList;
	friend class IVTEntry;
	friend class Event;
	friend class KernelEv;
	friend class Semaphore;

	friend void dispatch();

	Kernel();

	static Idle* idle;
	static PCB* running;

	static ThreadList* globalThreadList;

	static void interrupt timer(...);
	static interrupt void(*oldTimer)(...);

	volatile static unsigned dispatched;
	volatile static unsigned contextChangeRequested;
	volatile static unsigned lockCnt;

};

extern void tick();


#endif /* KERNEL_H_ */
