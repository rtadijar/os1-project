/*
 * Idle.h
 *
 *  Created on: May 28, 2019
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "Thread.h"

class Idle: public Thread{
		public:
			Idle(): Thread(4096, 1) {};
			void run() {
				while(1) {}
			}
};




#endif /* IDLE_H_ */
