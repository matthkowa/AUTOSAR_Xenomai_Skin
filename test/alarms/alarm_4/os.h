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

extern void StartOS (AppModeType mode);

extern void ShutdownOS (StatusType Error);

extern const AppModeType app1;
/********************************** TASK *********************************/

#define RUNNING 	0
#define WAITING 	1
#define READY		2
#define SUSPENDED	3
#define INVALID_TASK	-1


#define TASK(taskID) void taskID##_function()
#define DeclareTask(taskID) extern const TaskType taskID;

typedef uintptr_t TaskType;

typedef TaskType * TaskRefType;

typedef uintptr_t TaskStateType;

typedef TaskStateType * TaskStateRefType; 


extern StatusType ActivateTask(TaskType taskId);

extern StatusType TerminateTask(void);

extern StatusType ChainTask(TaskType taskId);

extern StatusType Schedule(void);

extern StatusType GetTaskID(TaskRefType taskId);

extern StatusType GetTaskState(TaskType taskId, TaskStateRefType state);

extern const TaskType t1;
extern const TaskType t2;

/****************************** END OF TASK ******************************/
/********************************** ALARM ********************************/
#define DeclareAlarm(alarmID) extern AlarmType alarmID;
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

extern StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info);

extern StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick);

extern StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle);

extern StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle);

extern StatusType CancelAlarm(AlarmType AlarmID);

extern const AlarmType alarm1;
extern const AlarmType alarm2;
extern const AlarmType alarm3;

/****************************** END OF ALARM *****************************/
/******************************** COUNTER ********************************/
#define DeclareCounter(counterID) extern const CounterType counterID;
typedef uintptr_t CounterType;
extern StatusType IncrementCounter(CounterType counterID);
extern StatusType GetCounterValue(CounterType CounterID,TickRefType Value);
extern StatusType GetElapsedValue(CounterType CounterID,TickRefType Value,TickRefType ElapsedValue);

extern const CounterType counter1;
/***************************** END OF COUNTER ****************************/
/******************************** RESOURCE *******************************/
typedef uintptr_t ResourceType;
#define DeclareResource(resourceID) extern const ResourceType resourceID;

extern StatusType GetResource(ResourceType ResID);

extern StatusType ReleaseResource(ResourceType ResID);

/**************************** END OF RESOURCE ****************************/


/********************************** HOOK *********************************/

typedef uintptr_t OSServiceIdType;
extern OSServiceIdType OSErrorGetServiceId(void);

/* Task */
#define OSServiceId_ActivateTask 		1
#define OSServiceId_TerminateTask 		2
#define OSServiceId_ChainTask 			3
#define OSServiceId_Schedule			4
#define OSServiceId_GetTaskID 			5
#define OSServiceId_GetTaskState		6

extern TaskType OSError_ActivateTask_TaskID(void);
extern TaskType OSError_ChainTask_TaskID(void);
extern TaskType OSError_GetTaskID_TaskID(void);
extern TaskType OSError_GetTaskState_TaskID(void);
extern TaskStateRefType OSError_GetTaskState_State(void);


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

extern AlarmType OSError_GetAlarmBase_AlarmID(void);
extern AlarmBaseRefType OSError_GetAlarmBase_Info(void);
extern AlarmType OSError_GetAlarm_AlarmID(void);
extern TickRefType OSError_GetAlarm_Tick(void);
extern AlarmType OSError_SetRelAlarm_AlarmID(void);
extern TickType OSError_SetRelAlarm_increment(void);
extern TickType OSError_SetRelAlarm_cycle(void);
extern AlarmType OSError_SetAbsAlarm_AlarmID(void);
extern TickType OSError_SetAbsAlarm_start(void);
extern TickType OSError_SetAbsAlarm_cycle(void);
extern AlarmType OSError_CancelAlarm_AlarmID(void);


/* OS */
#define OSServiceId_GetActiveApplicationMode	24
#define OSServiceId_StartOS			25
#define OSServiceId_ShutdownOS			26





/****************************** END OF HOOK ******************************/
 
#endif /* _XENOMAI_AUTOSAR_OS_H */
