#include "idlethre.h"
#include "pcb.h"
#include <iostream.h>
#include "macros.h"

IdleThread::IdleThread():Thread(1024, 0)
{
	
}

IdleThread::~IdleThread()
{
	myPCB->currentState = FINISHED;
}

void IdleThread::run()
{
	while(1);
}

void IdleThread::start()
{
	lock()
	myPCB->currentState = READY;
	myPCB->formStack();
	unlock()
}