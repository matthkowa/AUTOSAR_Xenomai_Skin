#ifndef _AUTOSAR_ISR_H
#define _AUTOSAR_ISR_H
#include "os_types.h"
#include "os_resource.h"

enum IsrCategory {
	ISR_CATEGORY_1,
	ISR_CATEGORY_2
};

typedef enum IsrCategory OsIsrCategory;

struct OsIsrResourceLock {
	float64 OsIsrResourceLockBudget;
	struct OsResource * OsIsrResourceLockResourceRef;
};

struct OsIsrTimingProtection {
	float64 OsIsrAllInterruptLockBugdet;
	float64 OsIsrExecutionBudget;
	float64 OsIsrInterruptLockBudget;
	float64 OsIsrTimeFrame;
	struct OsIsrResourceLock ** OsIsrResourceLock;

};

struct OsIsr {
	OsIsrCategory OsIsrCategory;
	struct OsResource ** OsIsrResourceRef;
	struct OsIsrTimingProtection * OsIsrTimingProtection;
};

#endif 
