#include "queue.h"
#include <iostream.h>

void Queue::put(PCB *p)
{
	lock()
	Elem *e = new Elem(p);
	if(!first) 
		first = e;
	else
		last->next = e;
	last = e;
	length++;	
	unlock()
}

PCB *Queue::get()
{
	if(length == 0) 
		return 0;
	lock()
	Elem *current = first;
	if(!current) return 0;
	first = first->next;
	PCB *pcb = current->pcb;
	delete current;
	length--;
	unlock()
	return pcb;
}

void Queue::empty()
{
	lock()
	Elem *old;
	while(first)
	{
		old = first;
		first = first->next;
		delete old;
	}
	length = 0;
	unlock()
}

int Queue::Length() const
{
	return length;
}