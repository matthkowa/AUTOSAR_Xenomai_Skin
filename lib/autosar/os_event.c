/**
 *
 *	\file os_event.c
 *	\brief Event implementation
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/

#include "os_event.h"
#include "os_task.h"

static struct OsTask * getTask(TaskType __ TaskID){
        struct OsTask * __osTask = NULL;
	if(__taskID <= TASK_COUNT && __taskID >= 1){	
		__osTask =  os.OsTask[__taskID-1];	
	}	
	return __osTask;

}

StatusType __SetEvent(TaskType __TaskID, EventMaskType __Mask){
        StatusType __ret = E_OK;
        struct OsTask * __osTask = getTask(__TaskID);
        if(__osTask == NULL){
#if OS_STATUS == OSOS_EXTENDED
                __ret = E_OS_ID;
#endif
                goto out;
        }
out : 
#if OS_ERROR_HOOK
	if(__ret){
		__SetTaskID(__taskID);
                __SetEventMaskType(__Mask
		__CheckError(__ret,OSServiceId_SetEvent);
	}
#endif
        return __ret;
}

StatusType __ClearEvent(EventMaskType __Mask){


}

StatusType __GetEvent(TaskType __TaskID,EventMaskRefType __Event){


}

StatusType __WaitEvent(EventMaskType __Mask){



}
