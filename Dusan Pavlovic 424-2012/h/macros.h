#ifndef _MACROS_H_
#define _MACROS_H_

typedef void interrupt (*InterruptRoutine) (...);
typedef char byte;
typedef enum { NEW, READY, BLOCKED, FINISHED } State;


#define lock() { asm { pushf; cli; } }

#define unlock() { asm { popf; } }
			
#define TRUE 1
#define FALSE 0


#endif