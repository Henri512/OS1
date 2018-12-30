#ifndef _pcb_h_
#define _pcb_h_

#include "queue.h"
#include "thread.h"
#include "macros.h"
#include <stdio.h>
class Queue;
class Semaphore;

const StackSize maxStackSize = 65636;


class PCB
{
	public:
		PCB(StackSize stackSize, Time timeSlice);
		~PCB();
		
		void formStack();
		void setMain();
		volatile State currentState;
		
		Queue *waitQueue;
		StackSize *stack;
		Thread *myThread;
		unsigned sp, ss, bp;
		unsigned pcbStackSize;
		
		static volatile unsigned demanded_context_change;
		Time pcbTimeSlice;		
};

#endif 