/*
 * KernelEv.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

typedef unsigned char IVTNo;

class PCB;

class KernelEv {

private:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void signal();
	void wait();

	int val;
	PCB* myPCB;
	IVTNo entry;

	friend class IVTEntry;
	friend class Event;
};


#endif /* KERNELEV_H_ */
