/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "Kernel.h"
#include "KernelEv.h"
#include "dos.h"


IVTEntry* IVTEntry::IVTET[256];


IVTEntry::IVTEntry(IVTNo ivtn, void interrupt (*newRoutine)(...)) : ivtNo(ivtn), myKernelEv(0) {
	IVTET[ivtNo] = this;
	
#ifndef BCC_BLOCK_IGNORE
	hard_lock
	oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
	hard_unlock
#endif
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	hard_lock
	setvect(ivtNo,oldRoutine);
	hard_unlock
#endif
}


void IVTEntry::signal() {
	myKernelEv->signal();
}
