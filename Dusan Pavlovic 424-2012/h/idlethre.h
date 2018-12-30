#ifndef _IDLETHREAD_H_
#define _IDLETHREAD_H_
#include "thread.h"

class Thread;

class IdleThread : public Thread
{	
	public:	
		IdleThread();
		~IdleThread();
		void run();
		void start();
};

#endif