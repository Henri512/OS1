#include "pcb.h"
#include <dos.h>
#include "schedule.h"
#include <iostream.h>
#include "system.h"


PCB::PCB(StackSize stackSize, Time timeSlice)
{
	lock()
	if(stackSize <= 0 || stackSize > maxStackSize) stackSize = defaultStackSize;
	pcbTimeSlice = timeSlice;
	pcbStackSize = stackSize / 2;
	currentState = NEW;
	waitQueue = new Queue();
	unlock()
}

PCB::~PCB()
{
	//lock()// pod lock() - om zbog operatora delete
	if(waitQueue != 0)
		delete waitQueue;
	if(stack != 0)
		delete [] stack;
	//lock()
}


void PCB::formStack()
{	
	lock()
	StackSize *tempStack = new StackSize[pcbStackSize];	//pod lock() - om zbog operatora new
	this->stack = tempStack;
	
	static volatile newSP, newSS, newBP, newCS, newIP, oldBP, oldSP, oldSS;
	
	newSP = sp = FP_OFF(stack + pcbStackSize);
	newSS = ss = FP_SEG(stack + pcbStackSize);
	newBP = bp = FP_OFF(stack + pcbStackSize);
	
	newIP = FP_OFF(&(System::wrappRun));
	newCS = FP_SEG(&(System::wrappRun));
	asm{
		mov oldSS, ss
		mov oldSP, sp
		mov oldBP, bp
		
		mov sp, WORD PTR newSP
		mov ss, WORD PTR newSS
		mov bp, WORD PTR newBP		
		
		pushf
		pop ax
		or ax, 0x200
		push ax
		
		mov ax, WORD PTR newCS
		push ax
		mov ax, WORD PTR newIP
		push ax
		
		xor ax, ax // brze se izvrsava od mov ax, 0	
		
		push ax
		push bx
		push cx
		push dx
		push es
		push ds
		push si
		push di
		push bp
	}
	
	asm{
		mov newSP, sp
		mov sp, oldSP
		mov newSS, ss
		mov ss, oldSS
		mov newBP, bp
		mov bp, oldBP
	}
	this->sp = newSP;
	this->bp = newBP;
	this->ss = newSS;
	unlock()
}
