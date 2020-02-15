/*
 * HanList.h
 *
 *  Created on: Sep 13, 2019
 *      Author: OS1
 */

#ifndef HANLIST_H_
#define HANLIST_H_

#include "thread.h"

class HandlerList {
private:

	friend class PCB;
	friend class Kernel;

	struct Elem{
		SignalHandler handler;
		Elem* next, *prev;
		Elem(SignalHandler hnd) : handler(hnd), next(0), prev(0) {};
	};

	void add(SignalHandler handler);
	void swap(SignalHandler handler1, SignalHandler handler2);
	void handle();
	void clear();

	HandlerList();
	HandlerList(HandlerList* list);
	virtual ~HandlerList();

	Elem* head, *tail;
};

#endif /* HANLIST_H_ */
