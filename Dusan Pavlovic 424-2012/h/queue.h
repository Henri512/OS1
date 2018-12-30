#ifndef _queue_h_
#define _queue_h_

#include "pcb.h"
#include "macros.h"

class PCB;

class Queue
{
	public:
		Queue():first(0), last(0), length(0) { }
		~Queue() { empty(); }
		void put(PCB *);
		PCB *get();
		int Length() const;
	private:
		struct Elem
		{
			PCB *pcb;
			Elem *next;
			Elem(PCB *p, Elem *n = 0)
			{				
				pcb = p;
				next = n;				
			}
		};
		Elem *first, *last;
		unsigned int length;
		void empty();
};

#endif