/*
 * KernelEv.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "KernelEv.h"
#include "Kernel.h"
#include "IVTEntry.h"
#include "SCHEDULE.h"
#include "PCB.h"



KernelEv::KernelEv(IVTNo ivtNo): entry(ivtNo), val(0) {
	lock
	if(IVTEntry::IVTET[entry] != 0)  {
		myPCB = Kernel::running;
		IVTEntry::IVTET[entry]->myKernelEv = this;
	}
	unlock

}

KernelEv::~KernelEv() {
	IVTEntry::IVTET[entry]->myKernelEv = 0;
}


void KernelEv::signal() {
	lock
	if(++val == 0) {
		myPCB->state == PCB::READY;
		Scheduler::put(myPCB);
		unlock
	}
	else {
		unlock
	}
}

void KernelEv::wait() {
	if(Kernel::running == myPCB) {
		lock
		if(--val == -1) {
			myPCB->state = PCB::BLOCKED;
			unlock;
			if(!Kernel::locked()) dispatch();
		}
		else {
			unlock;
		}
	}
}





