#ifndef _AUTOSAR_EVENT_H
#define _AUTOSAR_EVENT_H
#include "os_types.h"

struct OsEvent {
	uint64 OsEventMask;
};


StatusType __SetEvent(TaskType TaskID, EventMaskType Mask);
StatusType __ClearEvent(EventMaskType Mask);
StatusType __GetEvent(TaskType TaskID,EventMaskRefType Event);
StatusType __WaitEvent(EventMaskType Mask);


#endif
