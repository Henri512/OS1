#ifndef _KERNELSEM_H_
#define _KERNELSEM_H_

#include "queue.h"

class KernelSem
{
	public:
		KernelSem(int val);
		void signal();
		void wait();
		~KernelSem();
		int val() const;
		friend class Semaphore;
	private:
		int value;
		Queue *queue;
};

#endif