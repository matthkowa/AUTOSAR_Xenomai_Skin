#ifndef _AUTOSAR_ISR_H
#define _AUTOSAR_ISR_H
#include "os_types.h"
#include "os_resource.h"
#include "app/app_define.h"
#include <rtdm/rtdm.h>
#include <linux/ioctl.h>
enum IsrCategory {
	ISR_CATEGORY_1,
	ISR_CATEGORY_2
};

typedef enum IsrCategory OsIsrCategory;

/*struct OsIsrResourceLock {
	float64 OsIsrResourceLockBudget;
	struct OsResource * OsIsrResourceLockResourceRef;
};*/

/*struct OsIsrTimingProtection {
	float64 OsIsrAllInterruptLockBugdet;
	float64 OsIsrExecutionBudget;
	float64 OsIsrInterruptLockBudget;
	float64 OsIsrTimeFrame;
	struct OsIsrResourceLock ** OsIsrResourceLock;

};*/


struct OsIsr {
	OsIsrCategory OsIsrCategory;
	struct OsResource ** OsIsrResourceRef;
         char name[XNOBJECT_NAME_LEN];
        int fd;
        void (*handler)(void);
//	struct OsIsrTimingProtection * OsIsrTimingProtection;
};


ISRType __GetISRID(void);

void __EnableAllInterrupts(void);
void __DisableAllInterrupts(void);
void __ResumeAllInterrupts(void);
void __SuspendAllInterrupts(void);
void __ResumeOSInterrupts(void);
void __SuspendOSInterrupts(void);

void __InitISR(ISRType __ISRID);
void __StopIsr(ISRType __ISRID);
#endif 
