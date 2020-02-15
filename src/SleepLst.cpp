/*
 * SleepLst.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: OS1
 */

#include "SleepLst.h"
#include "ThrList.h"
#include "Thread.h"
#include "PCB.h"
#include "Kernel.h"
#include "SCHEDULE.H"
#include "KernSem.h"

SleepingList::SleepingList() : head(0), tail(0) {}

void SleepingList::add(ThreadList* threadList, KernelSem* myKernelSem){
	lock
	if(head == 0) {
		head = new Elem(threadList, myKernelSem);
	}
	else if(tail == 0){
		tail = new Elem(threadList, myKernelSem);
		head->next = tail;
		tail->prev = head;
	}
	else {
		tail->next = new Elem(threadList, myKernelSem);
		tail->next->prev = tail;
		tail = tail->next;
	}
	unlock
}

void SleepingList::tick() {

	if(head == 0) return;

	Elem *curr = head;

	while(curr != 0) {
		ThreadList::Elem *prev_thread = 0, *curr_thread = curr->list->head;

		while(curr_thread != 0) {
			if(--curr_thread->thread->myPCB->waitLeft == 0) {
				ThreadList::Elem *temp = curr_thread;

				curr_thread->thread->myPCB->state = PCB::READY;
				Scheduler::put(curr_thread->thread->myPCB);

				if(prev_thread != 0) prev_thread->next = curr_thread->next;
				curr_thread = curr_thread->next;

				if(temp == curr->list->head) curr->list->head = curr_thread;
				else if(temp == curr->list->tail) curr->list->tail = prev_thread;
				if(curr->list->head == 0) curr->list->tail == 0;

				curr->myKernSem->val++;

				delete temp;
			}
			else {
				prev_thread = curr_thread;
				curr_thread = curr_thread->next;
			}
		}

		curr = curr->next;
	}
}


SleepingList::~SleepingList() {
	// TODO Auto-generated destructor stub
}

