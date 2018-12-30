#ifndef _thread_h_
#define _thread_h_

class PCB;

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2 x 55ms
typedef int ID;
extern void tick();

class PCB; // Kernel's implementation of a user's thread
class Thread
{
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	
protected:
	friend class PCB;
	friend class System;
	friend class IdleThread;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() { }
	
private:
	PCB *myPCB;	
};

void dispatch();

#endif