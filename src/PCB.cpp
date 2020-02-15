/*
 * PCB.cpp
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#include "PCB.h"
#include "Thread.h"
#include "ThrList.h"
#include "Kernel.h"
#include "Idle.h"
#include "SigList.h"
#include "HanList.h"
#include "dos.h"

unsigned PCB::idGen = 0;
unsigned PCB::globallyBlockedSignals = 0;

PCB::PCB(Thread* threadInCreation = 0, StackSize stackSz = defaultStackSize, Time timeSl = defaultTimeSlice) : thread(threadInCreation), timeSlice(timeSl), timeLeft(timeSl), stackSize(stackSz), waitLeft(0) {
	id = idGen++;
	state = UNSTARTED;

	parent = Kernel::running;

	lock
	Kernel::globalThreadList->add(threadInCreation);
	waitingThreads = new ThreadList(0);
	signals = new SignalList(this);

	if(parent == 0){
		for(int i = 0; i < 16; i ++) {
			handlers[i] = new HandlerList();
			if(i == 0) handlers[i]->add(PCB::zeroSignalHandler);
		}
		blockedSignals = 0;
	}
	else {
		for(int i = 0; i < 16; i ++) handlers[i] = new HandlerList(parent->handlers[i]);
		blockedSignals = parent->blockedSignals;
	}
	unlock

	if (stackSize > 0x10000) stackSize = 0x10000;
	ss = sp = 0;
	stack = 0;
}

void PCB::runWrapper() {
	Kernel::running->thread->run();

	lock
	Kernel::running->state = DEAD;
	Kernel::running->waitingThreads->clearAndSchedule();
	if(Kernel::running->parent != 0) Kernel::running->parent->signal(1);
	Kernel::running->signal(2);
	Kernel::running->handleSignals();
	unlock

	dispatch();
}

void PCB::init() {
	int size = stackSize>>1;

	lock
	stack = new unsigned[size];
	unlock

	stack[size-1] = 0x200;

#ifndef BCC_BLOCK_IGNORE
	stack[size-2] = FP_SEG(PCB::runWrapper);
	stack[size-3] = FP_OFF(PCB::runWrapper);
	ss = FP_SEG(stack+size-12);
	sp = FP_OFF(stack+size-12);
#endif

	state=PCB::READY;
}

void PCB::waitToComplete() {
	lock
	if (this != Kernel::running && this != Kernel::idle && state != PCB::DEAD) {
	waitingThreads->add(Kernel::running->thread);
	Kernel::running->state = PCB::WAITING;
	unlock
	dispatch();
	}
	else {
	unlock
	}
}

void PCB::signal(SignalId signal) {
	lock
	if(signal < 16 && signals != 0) signals->add(signal);
	unlock
}

void PCB::registerHandler(SignalId signal, SignalHandler handler) {
	lock
	int sig = signal;

	if(signal < 16 && handlers[signal] != 0)	handlers[signal]->add(handler);
	unlock
}

void PCB::unregisterAllHandlers(SignalId id) {
	lock
	if(id < 16 && handlers[id] != 0) handlers[id]->clear();
	unlock
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	lock
	if(id < 16 && handlers[id] != 0) handlers[id]->swap(hand1, hand2);
	unlock
}

void PCB::blockSignal(SignalId signal) {
	blockedSignals |= (1 << signal);
}

void PCB::blockSignalGlobally(SignalId signal) {
	globallyBlockedSignals |= (1 << signal);
}

void PCB::unblockSignal(SignalId signal) {
	blockedSignals &= ~(1 << signal);
}

void PCB::unblockSignalGlobally(SignalId signal) {
	globallyBlockedSignals &= ~(1 << signal);
}

void PCB::zeroSignalHandler() {
	PCB* toDelete = Kernel::running;

	if(toDelete->state != DEAD){

	toDelete->waitingThreads->clearAndSchedule();
	toDelete->state = DEAD;


	delete[] toDelete->stack;

	delete toDelete->waitingThreads;

	delete toDelete->signals;


	toDelete->stack = 0;

	toDelete->waitingThreads = 0;

	toDelete->signals = 0;

	}
	Kernel::lockCnt = 0;
	dispatch();

}

void PCB::handleSignals() {
	int id;
	if(Kernel::running->id == 3) {

		SignalList::Elem* curr = signals->head;
		while(curr != 0) {
			int b = curr->signalId;
			curr = curr->next;
		}
	}

	while((id = signals->nextSignal()) != -1) {
		handlers[id]->handle();
	}
}


PCB::~PCB() {
	lock
	if(stack != 0) delete[] stack;
	if (waitingThreads != 0) delete waitingThreads;
	if (signals != 0) delete signals;
	for(int i = 0; i < 16; i ++) delete handlers[i];
	unlock
}
