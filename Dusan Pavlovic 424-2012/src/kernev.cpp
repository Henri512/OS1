#include "kernev.h"
#include "system.h"
#include "IVTEntry.h"


KernelEv::KernelEv(IVTNo ivt)
{
	binarySem = new KernelSem(0);
	isInterrupted = 0;
	threadWhoCalls = (PCB *)System::running;
	ivtEntry = IVTEntry::getEntry(ivt);
	ivtNo = ivt;
	ivtEntry->assignEv(this);
}

KernelEv::~KernelEv()
{
	delete binarySem;
}

void KernelEv::signal()
{
	if(!System::isActive)//ako sistem nije pokrenut
		return;
	if(binarySem->val() == 0)
	{
		isInterrupted = 1;
		return;
	}
	else
		binarySem->signal();
	return;
}

void KernelEv::wait()
{
	if(!System::isActive)//ako sistem nije pokrenut
		return;
	if(System::running != (volatile PCB *)threadWhoCalls)//ako je wait pozvala nit koja nije kreirala dogadjaj
		return;
	if(isInterrupted == 0 )//nit se blokira i ceka na prekid
	{
		binarySem->wait();
		return;
	}
	else
	{
		isInterrupted = 0;
		return;
	}
}