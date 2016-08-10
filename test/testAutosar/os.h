/**
 * \file alarm.h
 * \brief Programme de tests.
 * \author Matthieu.K
 * \version 0.1
 * \date 24 Mai 2016
 *
 * Programme de test pour le skin Autosar sous Xenomai
 *
 */
#ifndef _XENOMAI_AUTOSAR_OS_H
#define _XENOMAI_AUTOSAR_OS_H

#include <stdint.h>

typedef uintptr_t AppModeType;

typedef enum
{
	E_OK,
	E_OS_ACCESS,
	E_OS_CALLEVEL,
	E_OS_ID,
	E_OS_LIMIT,
	E_OS_NOFUNC,
	E_OS_RESOURCE,
	E_OS_STATE,
	E_OS_VALUE
}
StatusType;

void StartOS (AppModeType mode);

/********************************** TASK *********************************/

#define RUNNING 	0
#define WAITING 	1
#define READY		2
#define SUSPENDED	3
#define INVALID_TASK	-1

#define T_LOPRIO  0
#define T_HIPRIO  250



#define TASK(taskID) void taskID##_function()

typedef uintptr_t TaskType;

typedef TaskType * TaskRefType;

typedef uintptr_t TaskStateType;

typedef TaskStateType * TaskStateRefType; 


//DeclareTask(TaskType TaskId);

StatusType ActivateTask(TaskType taskId);

StatusType TerminateTask(void);

StatusType ChainTask(TaskType taskId);

StatusType Schedule(void);

StatusType GetTaskID(TaskRefType taskId);

StatusType GetTaskState(TaskType taskId, TaskStateRefType state);

extern const TaskType t1;
extern const TaskType t2;

/****************************** END OF TASK ******************************/
/********************************** ALARM ********************************/
typedef uintptr_t TickType;
typedef TickType * TickRefType;
typedef struct AlarmBaseType AlarmBaseType;
struct AlarmBaseType{
	TickType maxallowedvalue;
	TickType ticksperbase;
	TickType mincycle;
};

typedef AlarmBaseType * AlarmBaseRefType;
typedef uintptr_t AlarmType;

StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info);

StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick);

StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle);

StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle);

StatusType CancelAlarm(AlarmType AlarmID);
/****************************** END OF ALARM *****************************/
/********************************** HOOK *********************************/

typedef uintptr_t OSServiceIdType;
OSServiceIdType OSErrorGetServiceId(void);

/* Task */
#define OSServiceId_ActivateTask 		1
#define OSServiceId_TerminateTask 		2
#define OSServiceId_ChainTask 			3
#define OSServiceId_Schedule			4
#define OSServiceId_GetTaskID 			5
#define OSServiceId_GetTaskState		6

TaskType OSError_ActivateTask_TaskID(void);
TaskType OSError_ChainTask_TaskID(void);
TaskType OSError_GetTaskID_TaskID(void);
TaskType OSError_GetTaskState_TaskID(void);
TaskStateRefType OSError_GetTaskState_State(void);


/* Interrupt */
#define OSServiceId_EnableAllInterrupts 	7
#define OSServiceId_DisableAllInterrupts	8
#define OSServiceId_ResumeAllInterrupts		9
#define OSServiceId_SuspendAllInterrupts	10
#define OSServiceId_ResumeOSInterrupts		11
#define OSServiceId_SuspendOSInterrupts		12

/* Resource */
#define OSServiceId_GetResource			13
#define OSServiceId_ReleaseResource		14

/* Event */
#define OSServiceId_SetEvent			15
#define OSServiceId_ClearEvent			16
#define OSServiceId_GetEvent			17
#define OSServiceId_WaitEvent			18

/* Alarm */
#define OSServiceId_GetAlarmBase		19
#define OSServiceId_GetAlarm			20
#define OSServiceId_SetRelAlarm			21
#define OSServiceId_SetAbsAlarm			22
#define OSServiceId_CancelAlarm			23

AlarmType OSError_GetAlarmBase_AlarmID(void);
AlarmBaseRefType OSError_GetAlarmBase_Info(void);
AlarmType OSError_GetAlarm_AlarmID(void);
TickRefType OSError_GetAlarm_Tick(void);
AlarmType OSError_SetRelAlarm_AlarmID(void);
TickType OSError_SetRelAlarm_increment(void);
TickType OSError_SetRelAlarm_cycle(void);
AlarmType OSError_SetAbsAlarm_AlarmID(void);
TickType OSError_SetAbsAlarm_start(void);
TickType OSError_SetAbsAlarm_cycle(void);
AlarmType OSError_CancelAlarm_AlarmID(void);


/* OS */
#define OSServiceId_GetActiveApplicationMode	24
#define OSServiceId_StartOS			25
#define OSServiceId_ShutdownOS			26





/****************************** END OF HOOK ******************************/
 
#endif /* _XENOMAI_AUTOSAR_OS_H */
