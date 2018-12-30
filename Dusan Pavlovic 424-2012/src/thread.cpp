#include <dos.h>
#include "system.h"
#include "thread.h"
#include "macros.h"
#include "pcb.h"
#include "schedule.h"

void Thread::waitToComplete()
{
	lock()	
	
	if(!System::isActive)
	{
		unlock()
		return;
	}
	
	if(myPCB == (PCB *)System::running || this == System::starting)
	{
		unlock()
		return;
	}
	
	if(myPCB->currentState == FINISHED || myPCB->currentState == NEW)
	{
		unlock()
		return;
	}
	
	if(this == System::idleThread)
	{
		unlock()
		return;
	}
	
	System::running->currentState = BLOCKED;
	myPCB->waitQueue->put((PCB *)System::running);
	unlock()
	dispatch();
}

Thread::Thread(StackSize stackSize, Time timeSlice)
{
	lock()
	myPCB = new PCB(stackSize, timeSlice);
	myPCB->myThread = this;
	unlock()
}


Thread::~Thread()
{
	waitToComplete();
	lock()
	delete myPCB;
	unlock()	
}

void dispatch()
{
	System::dispatch();
}

void Thread::start()
{
	lock()
	myPCB->formStack();
	myPCB->currentState = READY;
	Scheduler::put((PCB *)myPCB);
	unlock()
}