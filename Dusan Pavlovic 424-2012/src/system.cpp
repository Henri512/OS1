#include "system.h"
#include "macros.h"
#include "schedule.h"
#include "thread.h"
#include <iostream.h>
#include <dos.h>
#include "event.h"
#include "IVTEntry.h"

volatile byte System::demanded_context_change = 0;
static volatile unsigned oldTimerSEG, oldTimerOFF;
static volatile unsigned lockFlag = 0;
volatile PCB *System::running = 0;
IdleThread* System::idleThread = 0;
Thread *System::starting = 0;
byte System::counter = 1;
byte System::isActive = 0;
static unsigned int tempSEG, tempOFF;

//f-ja timer se smesta na ulaz 08h a stara prekidna rutina na 60h
void System::init()
{
	lock()
	tempSEG = FP_SEG(&timer);
	tempOFF = FP_OFF(&timer);
	asm{		
		push es
		push ax
		
		mov ax, 0
		mov es, ax
		
		mov ax, WORD PTR es:0x22
		mov WORD PTR oldTimerSEG, ax
		mov ax, WORD PTR es:0x20
		mov WORD PTR oldTimerOFF, ax
		
		mov  ax, WORD PTR tempSEG
		mov WORD PTR es:0x22, ax
		mov  ax, WORD PTR tempOFF
		mov WORD PTR es:0x20, ax
		
		mov ax, WORD PTR oldTimerSEG
		mov WORD PTR es:0x182, ax
		mov ax, WORD PTR oldTimerOFF
		mov WORD PTR es:0x180, ax
		
		pop ax
		pop es
	}
	unlock()
}

void System::restore()
{
	lock()
	asm{	
		push es
		push ax
		
		mov ax, 0
		mov es, ax
		
		mov ax, WORD PTR oldTimerSEG
		mov WORD PTR es:0x22, ax
		mov ax, WORD PTR oldTimerOFF
		mov WORD PTR es:0x20, ax
		
		pop ax
		pop es
	}	
	unlock()
}

void interrupt System::timer()
{	
	static volatile unsigned tempSS, tempSP, tempBP;
	static volatile PCB *tempThread = 0;
	
	if(!demanded_context_change)
	{
		tick();
		asm int 0x60;
	}
	
	if(!isActive)
	{
		return;
	}	
		
	//ako nije sinhroni prekid i ako vreme izvrsavanja niti nije beskonacno
	if(!demanded_context_change && running->pcbTimeSlice != 0) 
		counter--;	
		
	//ako nije sinhroni prekid i vreme niti nije isteklo i nit se ne izvrsava beskonacno vratimo se
	if(!demanded_context_change && (counter > 0 && running->pcbTimeSlice != 0))
		return;
		
	//ako trenutna nit nije starting ili idle a stanje joj jeste READY onda je ubacimo u Scheduler
	if(running->currentState == READY && running != (PCB *)idleThread->myPCB)
		Scheduler::put((PCB *)running);
	
	demanded_context_change = 0;
		
	while(1)
	{
			tempThread = Scheduler::get();
			if(tempThread == 0)
				tempThread = idleThread->myPCB;
			if(tempThread->currentState != READY)
				continue;
			break;
	}
		asm{
			mov tempSP, sp
			mov tempSS, ss
			mov tempBP, bp
		}
		
		running->sp = tempSP;
		running->ss = tempSS;
		running->bp = tempBP;
		
		running = tempThread;
		
		tempSP = running->sp;
		tempSS = running->ss;
		tempBP = running->bp;
		
		asm{
			mov sp, tempSP
			mov ss, tempSS
			mov bp, tempBP
		}
		counter = running->pcbTimeSlice;
	return;
}

void System::dispatch()
{
	lock()
	demanded_context_change = 1;
	timer();
	demanded_context_change = 0;
	unlock()
}

void System::wrappRun()
{
	running->myThread->run();
	lock()
	running->currentState = FINISHED;
	PCB *aid;
	while(running->waitQueue->Length() > 0)
	{
		aid = running->waitQueue->get();
		aid->currentState = READY;
		Scheduler::put((PCB *)aid);
	}
	unlock()
	dispatch();
}

void System::startSystem()
{
	if(isActive)
		return;
	lock()
	starting = new Thread(4096, 0);
	starting->myPCB->currentState = READY;
	running = starting->myPCB;
	
	idleThread = new IdleThread();
	idleThread->start();
	
	IVTEntry::initializeEntries();
	
	isActive = 1;
	init();
	unlock()
}

void System::stopSystem()
{
	if(!isActive)
		return;
	if(running != starting->myPCB)
		return;
		
	lock()
	restore();
	isActive = 0;
	//IVTEntry::restoreIVT();
	IVTEntry::deleteEntries();
	delete starting;
	unlock()
}