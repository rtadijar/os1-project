/*
 * ThreadList.cpp
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#include "ThrList.h"
#include "Thread.h"
#include "PCB.h"
#include "Kernel.h"
#include "SCHEDULE.H"



ThreadList::ThreadList(Thread* thread) {
	if(thread != 0) {
		head = tail = new Elem(thread);
	}
	else {
		head = tail = 0;
	}
}

Thread* ThreadList::get() {
	if(head == 0) return 0;
	else {
		Thread* thread = head->thread;

		if(head == tail){
			delete head;
			head = tail = 0;
		}
		else {
			Elem* tmp = head;
			head = head->next;
			delete tmp;
		}

		return thread;
	}
}

Thread* ThreadList::findById(unsigned id) {
	Elem* curr = head;
	while(curr != 0 && curr->thread != 0) {
		if(curr->thread->myPCB->id == id) return curr->thread;
		curr = curr->next;
	}
	return (Thread*)-1;
}

void ThreadList::clear() {
	Elem* prev = head, *curr = head->next;

	while(prev != 0) {
		delete prev;
		prev = curr;
		curr = curr->next;
	}
	head = tail = 0;


}

void ThreadList::clearAndSchedule() {
	Elem* prev = head, *curr = head->next;

	while(prev != 0) {
		prev->thread->myPCB->state = PCB::READY;
		Scheduler::put(prev->thread->myPCB);

		delete prev;
		prev = curr;
		curr = curr->next;
	}
	head = tail = 0;
}


void ThreadList::add(Thread* thread) {
	if(head == 0) {
		head = tail = new Elem(thread);
	}
	else if(head->next == 0) {
		head->next = tail = new Elem(thread);
	}
	else {
		tail = tail->next = new Elem(thread);
	}
}


ThreadList::~ThreadList() {
	lock
	clear();
	unlock
}
