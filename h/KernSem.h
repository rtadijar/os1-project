/*
 * KernSem.h
 *
 *  Created on: Aug 18, 2019
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_

class ThreadList;
class SleepingList;

typedef unsigned int Time;


class KernelSem {
private:

	friend class Semaphore;
	friend class Kernel;
	friend class Thread;
	friend class PCB;
	friend class SleepingList;

	KernelSem(int init);
	virtual ~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n);

	static SleepingList *sleepingList;

	int val;


	ThreadList* blockedTimed, *blockedIndef;


};

#endif /* KERNSEM_H_ */
