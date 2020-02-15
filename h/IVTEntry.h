/*
 * IVTEntry.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_


class KernelEv;

typedef unsigned char IVTNo;

class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, void interrupt (*newRoutine)(...));
	~IVTEntry();

	void signal();

	static IVTEntry* IVTET[];
	IVTNo ivtNo;
	void interrupt (*oldRoutine)(...);
	KernelEv* myKernelEv;
};

#endif /* IVTENTRY_H_ */
