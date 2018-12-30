#include "ivtentry.h"
#include "system.h"
#include "dos.h"
#include <iostream.h>

IVTEntry *IVTEntry::events[256] = { 0 };

IVTEntry::IVTEntry(IVTNo ivtno, InterruptRoutine ir)
{
	ivtNo = ivtno;
	if(events[ivtNo] == 0)
		events[ivtNo] = this;
	oldRout = getvect(ivtno);
	setvect(ivtno, ir);
}

void IVTEntry::initializeEntries()
{
	for(int i = 0; i < 256; i++)
		events[i] = 0;
}

IVTEntry *IVTEntry::getEntry(IVTNo ivtno)
{
	return events[ivtno];//nema uslova jer je opseg tipa IVTNo isti kao i broj ulaza u tabelu(256), 0..255
}

void IVTEntry::deleteEntries()
{
	for(int i = 0; i < 256; i++)
		delete events[i];
}

InterruptRoutine IVTEntry::getOldRout() const
{
	return oldRout;
}

void IVTEntry::signal()
{
	myEvent->signal();
}

IVTEntry::~IVTEntry()
{
	setvect(ivtNo, oldRout);
	delete myEvent;
}

void IVTEntry::assignEv(KernelEv *ke)
{
	myEvent = ke;
}

void IVTEntry::restoreIVT()
{
	for(unsigned int i = 0; i < 256; i++)
		if(events[i] != 0)
			setvect(i, events[i]->getOldRout());
}
