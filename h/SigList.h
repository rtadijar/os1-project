/*
 * SignalList.h
 *
 *  Created on: Sep 13, 2019
 *      Author: OS1
 */

#ifndef SIGNALLIST_H_
#define SIGNALLIST_H_

class SignalList {
public:
	friend class PCB;

	struct Elem{
			int signalId;
			Elem* next, *prev;
			Elem(int id) : signalId(id), next(0), prev(0) {};
	};

	SignalList(PCB* myPCB);

	void add(int signalId);
	int nextSignal();
	void clear();
	virtual ~SignalList();

private:

	Elem* head;
	Elem* tail;
	Elem* curr;

	PCB* myPCB;


};

#endif /* SIGNALLIST_H_ */
