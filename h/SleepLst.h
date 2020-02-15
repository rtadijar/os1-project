/*
 * SleepLst.h
 *
 *  Created on: Aug 19, 2019
 *      Author: OS1
 */

#ifndef SLEEPLST_H_
#define SLEEPLST_H_

class ThreadList;

class SleepingList {
private:
	friend class Kernel;
	friend class KernelSem;

	SleepingList();
	~SleepingList();

	struct Elem{
		ThreadList* list;
		Elem* next;
		Elem* prev;
		KernelSem* myKernSem;
		Elem(ThreadList* threadList, KernelSem* kernSem) : list(threadList), next(0), prev(0), myKernSem(kernSem) {}
	};

	void add(ThreadList* threadList, KernelSem* myKernelSem);

	void tick();

	void clear();

	Elem *head, *tail;


};

#endif /* SLEEPLST_H_ */
