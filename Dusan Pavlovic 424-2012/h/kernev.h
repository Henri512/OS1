#ifndef _KERNEL_EV_H
#define _KERNEL_EV_H

#include "kernsem.h"
#include "event.h"
#include "macros.h"

class KernelEv
{
	public:
		KernelEv(IVTNo ivt);
		~KernelEv();
		void signal();
		void wait();
	private:
		friend class Event;
		friend class IVTEntry;
		KernelSem *binarySem;
		IVTNo ivtNo;
		PCB *threadWhoCalls;
		byte isInterrupted;
		IVTEntry *ivtEntry;
};

#endif