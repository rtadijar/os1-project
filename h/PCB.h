/*
 * PCB.h
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"

class HandlerList;

typedef unsigned long StackSize;
typedef unsigned int Time;
typedef int ID;

class PCB {

private:
	friend class Thread;
	friend class Kernel;
	friend class ThreadList;
	friend class KernelSem;
	friend class SleepingList;
	friend class KernelEv;
	friend class SignalList;

	enum {
		UNSTARTED, READY, RUNNING, WAITING, BLOCKED, DEAD
	};

	PCB (Thread* threadInCreation, StackSize stackSize, Time timeSlice);
	
	~PCB();

	PCB* parent;
	Thread* thread;
	ThreadList* waitingThreads;

	static unsigned globallyBlockedSignals;
	unsigned blockedSignals;

	unsigned ss;
	unsigned sp;
	unsigned stackSize;
	unsigned* stack;


	unsigned timeSlice;
	unsigned timeLeft;

	int waitLeft;

	unsigned state;

	static unsigned idGen;
	unsigned id;

	void init();
	void waitToComplete();

	static void runWrapper();

	SignalList* signals;
	HandlerList* handlers[16];

	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	void handleSignals();

	static void zeroSignalHandler();
};

#endif /* PCB_H_ */
