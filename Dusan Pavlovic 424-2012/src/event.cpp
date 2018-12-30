#include "event.h"
#include "kernev.h"

Event::Event(IVTNo ivtno)
{
	lock()
	myImpl = new KernelEv(ivtno);
	unlock()
}
Event::~Event()
{
	lock()
	delete myImpl;
	unlock()
}

void Event::wait()
{
	lock()
	myImpl->wait();
	unlock()
}

void Event::signal()
{
	//lock()
	myImpl->signal();
	//unlock()
}