/*
 * KernSem.cpp
 *
 *  Created on: Aug 18, 2019
 *      Author: OS1
 */

#include "KernSem.h"
#include "semaphor.h"
#include "ThrList.h"
#include "SleepLst.h"
#include "Kernel.h"
#include "PCB.h"
#include "Thread.h"
#include "Idle.h"
#include "SCHEDULE.H"

KernelSem::KernelSem(int init) {
	val = init;

	lock
	blockedTimed = new ThreadList();
	blockedIndef = new ThreadList();

	if(KernelSem::sleepingList == 0){
		KernelSem::sleepingList = new SleepingList();
	}
	KernelSem::sleepingList->add(blockedTimed, this);
	unlock
}

KernelSem::~KernelSem() {
	lock
	blockedTimed->clearAndSchedule();
	blockedIndef->clearAndSchedule();

	SleepingList::Elem *curr = sleepingList->head;


	while(curr->list != blockedTimed) {
		curr = curr->next;
	}

	if(curr == sleepingList->head) sleepingList->head = curr->next;
	if(curr == sleepingList->tail) sleepingList->tail = curr->prev;

	if(curr->next != 0) curr->next->prev = curr->prev;
	if(curr->prev != 0) curr->prev->next = curr->next;

	delete curr;

	delete blockedTimed;
	delete blockedIndef;
	unlock
}


int KernelSem::wait(Time maxTimeToWait) {

	lock
	if(--val < 0) {
		Kernel::running->state = PCB::BLOCKED;

		if(maxTimeToWait > 0) {
			Kernel::running->waitLeft = maxTimeToWait;
			blockedTimed->add(Kernel::running->thread);

		}
		else if(maxTimeToWait == 0) {
			Kernel::running->waitLeft = -1;
			blockedIndef->add(Kernel::running->thread);
		}
		unlock
		if(!Kernel::locked()) dispatch();
	}
	else {
		unlock
	}

	if(Kernel::running->waitLeft == 0) {
		return 0;
	}
	else {
		return 1;
	}

}

int KernelSem::signal(int n) {

	lock
	Thread* thread;
	if(n == 0) {
		if(++val < 1) {
			if((int)(thread = blockedIndef->get()) != 0);
			else thread = blockedTimed->get();

			thread->myPCB->state = PCB::READY;

			Scheduler::put(thread->myPCB);

		}
		unlock
		return 0;
	}
	if(n > 0) {
		if(val >= 0){
			val += n;
			unlock
			return 0;
		}
		else{
			int retVal = -val ;
			if(retVal > n) retVal = n;

			for(int i = 0; i < retVal; i++) {
				if((int)(thread = blockedIndef->get()) != 0);
				else thread = blockedTimed->get();


				thread->myPCB->state = PCB::READY;
				Scheduler::put(thread->myPCB);
			}

			val += n;

			unlock
			return retVal;
		}
	}
	else {
		unlock
		return n;
	}

}










