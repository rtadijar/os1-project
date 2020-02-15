/*
 * HanList.cpp
 *
 *  Created on: Sep 13, 2019
 *      Author: OS1
 */

#include "HanList.h"
#include "Kernel.h"

HandlerList::HandlerList() : head(0), tail(0) {}

HandlerList::HandlerList(HandlerList* list) {
	head = tail = 0;
	Elem* curr = list->head;

	while(curr != 0) {
		this->add(curr->handler);
		curr = curr->next;
	}
}


void HandlerList::add(SignalHandler handler) {
	if(head == 0) {
		head = tail = new Elem(handler);
	}
	else if(head->next == 0) {
		head->next = tail = new Elem(handler);
		tail->prev = head;
	}
	else {
		tail->next = new Elem(handler);
		tail->next->prev = tail;
		tail = tail->next;
	}
}

void HandlerList::handle() {
	Elem* curr = head;

	while(curr != 0){
		(curr->handler)();
		curr = curr->next;
	}
}

void HandlerList::swap(SignalHandler handler1, SignalHandler handler2) {
	if(handler1 != handler2){
		Elem* temp1 = head, *temp2 = head;
		while(temp1 != 0) {
			if(temp1->handler == handler1 || temp2->handler == handler2) {
				temp2 = temp1->next;
				break;
			}
			temp1 = temp1->next;
		}

		while(temp2 != 0) {
			if(temp2->handler == handler1 || temp2->handler == handler2) break;
			temp2 = temp2->next;
		}

		if(temp1 != 0 && temp2 != 0) {
			SignalHandler temp = temp1->handler;
			temp1->handler = temp2->handler;
			temp2->handler = temp;
		}
	}
}

void HandlerList::clear() {
	if(head != 0){
		Elem* temp = head->next;

		while(temp != 0) {
			delete temp->prev;
			temp = temp->next;
		}
		delete temp;
	}
	head = tail = 0;
}

HandlerList::~HandlerList() {
	lock
	clear();
	unlock
}
