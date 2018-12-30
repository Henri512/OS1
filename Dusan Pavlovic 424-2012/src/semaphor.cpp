#include "semaphore.h"
#include "kernsem.h"
#include "system.h"
#include "pcb.h"
#include "thread.h"

Semaphore::Semaphore(int init)
{
	lock()
	myImpl = new KernelSem(init);
	unlock()
}

Semaphore::~Semaphore()
{
	lock()
	delete myImpl;
	unlock()
}

int Semaphore::val() const
{
	return myImpl->val();
}

void Semaphore::wait()
{
	lock()
	myImpl->wait();
	unlock()
}

void Semaphore::signal()
{
	lock()
	myImpl->signal();
	unlock()
}