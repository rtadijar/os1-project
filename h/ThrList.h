/*
 * ThreadList.h
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#ifndef THRLIST_H_
#define THRLIST_H_

class Thread;

class ThreadList {
public:

	struct Elem{
		Thread* thread;
		Elem* next;
		Elem(Thread* thr) : thread(thr), next(0) {};
	};

	ThreadList(Thread* thread = 0);

	void  add(Thread* thread);

	Thread* get();

	void clear();

	Thread* findById(unsigned id);

	virtual ~ThreadList();

private:

	Elem* head;
	Elem* tail;

	friend class PCB;
	friend class Kernel;
	friend class KernelSem;
	friend class SleepingList;

	void clearAndSchedule();
};



#endif /* THRLIST_H_ */
