/*
 * Event.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "Event.h"
#include "KernelEv.h"
#include "Kernel.h"



Event::Event(IVTNo ivtNo) {
	lock
	myImpl = new KernelEv(ivtNo);
	unlock
}

Event::~Event() {
	lock
	delete myImpl;
	unlock
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}
