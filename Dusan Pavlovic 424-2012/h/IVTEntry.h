#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#include "macros.h"
#include "kernev.h"

class IVTEntry
{
	public:
		IVTEntry(IVTNo ivtno, InterruptRoutine ir);
		~IVTEntry();
		
		void assignEv(KernelEv *event);
		
		InterruptRoutine getOldRout () const;
		static void deleteEntries();
		void signal();
		static void initializeEntries();
		static IVTEntry *getEntry(IVTNo ivtno);
		static void restoreIVT();
	private:
		friend class System;
		static IVTEntry *events[256];
		KernelEv *myEvent;
		IVTNo ivtNo;
		InterruptRoutine oldRout;
};

#define PREPAREENTRY(entryNum,oldRoutCall)\
void interrupt evRoutine##entryNum (...);\
void interrupt evRoutine##entryNum (...) {\
	IVTEntry::getEntry( entryNum ) -> signal ();\
	if(oldRoutCall) {\
		(*IVTEntry::getEntry( entryNum ) -> getOldRout ()) ();\
	dispatch();\
	}\
}\
IVTEntry entry##entryNum ( entryNum, evRoutine##entryNum );


#endif