/*
 * Kernel.cpp
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#include "Kernel.h"
#include "Thread.h"
#include "PCB.h"
#include "Idle.h"
#include "ThrList.h"
#include "KernSem.h"
#include "SleepLst.h"
#include "dos.h"
#include "SCHEDULE.H"
#include "iostream.h"

void interrupt(*Kernel::oldTimer)(...) = 0;
Idle*  Kernel::idle = 0;
PCB* Kernel::running = 0;
ThreadList* Kernel::globalThreadList = 0;

volatile unsigned Kernel::dispatched = 0;
volatile unsigned Kernel::contextChangeRequested = 0;
volatile unsigned Kernel::lockCnt = 0;

unsigned currSP, currSS;

int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	Kernel::initOS();
	int val = userMain(argc, argv);
	Kernel::restoreOS();

	cout << "Process finished with code: " << val << endl;

	return 0;
}

void Kernel::initOS(){

	Thread* mainThread = new Thread(0,0);
	Kernel::running = mainThread->myPCB;
	running->state = PCB::RUNNING;

	idle = new Idle();
	idle->start();

	Scheduler::get();

	globalThreadList = new ThreadList(mainThread);

#ifndef BCC_BLOCK_IGNORE
	hard_lock
	Kernel::oldTimer = getvect(0x8);
	setvect(0x60, oldTimer);
	setvect(0x8, &timer);
	hard_unlock
#endif
}

void Kernel::restoreOS(){
#ifndef BCC_BLOCK_IGNORE
	hard_lock
	setvect(0x8, oldTimer);
	hard_unlock
#endif
}

int Kernel::locked() {
	return lockCnt;
}

void interrupt Kernel::timer(...){
	if(dispatched == 0){
		(*oldTimer)();
		tick();
		KernelSem::sleepingList->tick();

		if (running->timeSlice != 0 && --running->timeLeft == 0) {
				if (lockCnt > 0) {
					contextChangeRequested = 1;
					return;
				}

		}
		else return;
	}
	dispatched = 0;
	contextChangeRequested = 0;

#ifndef BCC_BLOCK_IGNORE
		asm{
			mov currSS, ss
			mov currSP, sp
		}
#endif

	running->sp = currSP;
	running->ss = currSS;

	if (running != idle->myPCB && running->state == PCB::RUNNING) {
		running->state = PCB::READY;
		Scheduler::put(running);
	}

	if ((int)(running = Scheduler::get()) == 0) {
		running = idle->myPCB;
	}

	currSS = running->ss;
	currSP = running->sp;

	running->state = PCB::RUNNING;
	running->timeLeft = running->timeSlice;

#ifndef BCC_BLOCK_IGNORE
		asm{
			mov sp, currSP
			mov ss, currSS
		}
#endif

	lock
	running->handleSignals();
	unlock

}





