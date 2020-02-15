/*
 * SignalList.cpp
 *
 *  Created on: Sep 13, 2019
 *      Author: OS1
 */

#include "SigList.h"
#include "PCB.h"
#include "Kernel.h"

SignalList::SignalList(PCB* pcb) : head(0), tail(0), curr(0), myPCB(pcb) {}

void SignalList::add(int signalId) {
	if(head == 0) {
		curr = head = tail = new Elem(signalId);
	}
	else if(head->next == 0) {
		head->next = tail = new Elem(signalId);
		tail->prev = head;
	}
	else {
		tail->next = new Elem(signalId);
		tail->next->prev = tail;
		tail = tail->next;
	}
}

int SignalList::nextSignal() {


	if(curr == 0) {
		curr = head;
		return -1;
	}


	while((1 << curr->signalId)&PCB::globallyBlockedSignals || (1 << curr->signalId)&myPCB->blockedSignals) {
		curr = curr->next;
		if(curr == 0) {
			curr = head;
			return -1;
		}
	}

	if(curr == head) {
		head = curr->next;
	}
	if(curr == tail) {
		tail = curr->prev;
	}

	if(curr->prev) curr->prev->next = curr->next;
	if(curr->next) curr->next->prev = curr->prev;

	int ret = curr->signalId;


	Elem *temp = curr;

	curr = curr->next;

	delete temp;

	return ret;
}

void SignalList::clear() {

	Elem *temp = head->next;

	while(temp) {
		delete temp->prev;
		temp = temp->next;
	}

	delete head;
}


SignalList::~SignalList() {
	lock
	clear();
	unlock
}

