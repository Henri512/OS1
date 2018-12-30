#include "kernsem.h"
#include "thread.h"
#include "pcb.h"
#include "system.h"
#include "macros.h"
#include "schedule.h"

KernelSem::KernelSem(int val)
{
	value = val;
	queue = new Queue();
}

KernelSem::~KernelSem()
{
	delete queue;
}

void KernelSem::signal()
{
	if(!System::isActive)
		return;
	value++;
	if(value <= 0)
	{
		PCB *p = queue->get();
		p->currentState = READY;
		Scheduler::put((PCB*)p);
	}
}

void KernelSem::wait()
{
	if(!System::isActive)
		return;
	value--;
	if(value < 0)
	{
		System::running->currentState = BLOCKED;
		queue->put((PCB *)System::running);
		System::dispatch();
	}
}

int KernelSem::val() const
{
	return value;
}