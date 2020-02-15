/*
 * Thread.cpp
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#include "Thread.h"
#include "PCB.h"
#include "Kernel.h"
#include "ThrList.h"
#include "SCHEDULE.H"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock
}

Thread::~Thread() {
 	waitToComplete();
 	lock
	delete myPCB;
	unlock
}

void Thread::start(){
	lock
	myPCB->init();
	Scheduler::put(myPCB);
	unlock
}

void Thread::waitToComplete(){
	myPCB->waitToComplete();
}


void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	hard_lock
#endif
	Kernel::dispatched = 1;
	Kernel::timer();
#ifndef BCC_BLOCK_IGNORE
	hard_unlock
#endif
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId(){
	return Kernel::running->id;
}

Thread* Thread::getThreadById(ID id){
	return Kernel::globalThreadList->findById(id);
	return 0;
}

void Thread::signal(SignalId signal) {
	if(signal == 0 || signal > 2) myPCB->signal(signal);
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	myPCB->registerHandler(signal, handler);
}

void Thread::unregisterAllHandlers(SignalId id) {
	myPCB->unregisterAllHandlers(id);
}

void Thread::swap(SignalId signal, SignalHandler hand1, SignalHandler hand2) {
	myPCB->swap(signal, hand1, hand2);
}

void Thread::blockSignal(SignalId signal) {
	myPCB->blockSignal(signal);
}

void Thread::blockSignalGlobally(SignalId signal) {
	PCB::blockSignalGlobally(signal);
}

void Thread::unblockSignal(SignalId signal) {
	myPCB->unblockSignal(signal);
}

void Thread::unblockSignalGlobally(SignalId signal) {
	PCB::unblockSignalGlobally(signal);
}

