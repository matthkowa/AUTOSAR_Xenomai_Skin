#ifndef _AUTOSAR_SPINLOCK_H
#define _AUTOSAR_SPINLOCK_H
#include "os_types.h"
#include "os_application.h"
typedef struct OsSpinlock OsSpinlock;

struct OsSpinlock {
	EcucEnumerationParamDef OsSpinlockLockMethode;
	struct OsApplication * OsSpinlockAccessingApplication;
	struct OsSpinlock * OsSpinlockSuccessor;	
};


#endif
