#include "pcb.h"
#include "system.h"
#include "idlethre.h"
#include "macros.h"
#include "queue.h"
#include "user.h"
#include <iostream.h>
#include <stdio.h>

extern int userMain(int argc, char *argv[]);

int main(int argc, char *argv[])
{	
	System::startSystem();
	
	int r = userMain(argc, argv);
	
	System::stopSystem();
			
	return r;
}