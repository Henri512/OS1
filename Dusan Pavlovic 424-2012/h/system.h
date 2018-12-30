#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#include "macros.h"
#include "pcb.h"
#include "idlethre.h"
#include "thread.h"
#include "queue.h"
#include "event.h"

class System
{
	public:
		static void dispatch();
		static void init();
		static void restore();	
		static void startSystem();
		static void stopSystem();
	private:
		friend class Thread;
		friend class PCB;
		friend class KernelSem;
		friend class KernelEv;
		friend class IdleThread;
		friend class IVTEntry;
		
		static volatile byte demanded_context_change;		
		static IdleThread *idleThread;
		static Thread *starting;
		static volatile PCB *running;
		
		static byte counter;
		static byte isActive;
		
		static void interrupt timer();
		static void wrappRun();
};

#endif