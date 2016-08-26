/**
 *
 *	\file os_hook.h
 *	\brief Hook header
 *	\author  Matthieu K.
 *	\version 0.1
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_HOOK_H
#define _AUTOSAR_HOOK_H
#include "app/app_define.h"
#include <autosar/os.h>
#include <copperplate/threadobj.h>
#include <copperplate/cluster.h>
#include <copperplate/eventobj.h>
#include "os_types.h"


#define OS_STARTUP_HOOK_FLAG 			(1<<0)
#define OS_SHUTDOWN_HOOK_FLAG 			(1<<1)
#define OS_ERROR_HOOK_FLAG 			(1<<2)
#define OS_PRE_TASK_HOOK_FLAG 			(1<<3)
#define OS_POST_TASK_HOOK_FLAG 			(1<<4)


struct OsHookXenomai {
	char name[XNOBJECT_NAME_LEN]; 		/*!< Name of the task */
	struct threadobj thobj;			/*!< Thread object (Xenomai) */
	void (*entry)(void);			/*!< Function to call */
	struct clusterobj cobj;			/*!< Cluster object (Xenomai) */
	struct eventobj evobj;
	StatusType Error;
	int flag;
};

struct OsHookXenomai * OsHook_table[5];

OSServiceIdType _OSErrorGetServiceId;

StatusType __CreateHook(unsigned int flag);

StatusType __ActivateOsHook(unsigned int flag,StatusType error);


void __StopHook(unsigned int __flag);

#if OS_STARTUP_HOOK
	extern void StartupHook(void);
#endif
#if OS_SHUTDOWN_HOOK
	extern void ShutdownHook(StatusType Error);
#endif
#if OS_ERROR_HOOK

	extern void ErrorHook(StatusType Error);
	void __CheckError(StatusType ret,OSServiceIdType ServiceId);
	
	TaskType _TaskID;
	void __SetTaskID(TaskType TaskID);

	TaskRefType _TaskRefID;
	void __SetTaskRefID(TaskRefType taskRefID);
	
	TaskStateRefType _State;
	void __SetTaskState(TaskStateRefType State);

	AlarmType _AlarmID;
	void __SetAlarmID(AlarmType AlarmID);

	AlarmBaseRefType _Info;
	void __SetInfo(AlarmBaseRefType Info);

	TickRefType _Tick;
	void __SetTick(TickRefType Tick);

	TickType _increment;
	void __Setincrement(TickType increment);
	
	TickType _cycle;
	void __Setcycle(TickType cycle);

	TickType _start;
	void __Setstart(TickType start);

#endif
#if OS_PRE_TASK_HOOK
	extern void PreTaskHook(void);
#endif
#if OS_POST_TASK_HOOK
	extern void PostTaskHook(void);
#endif




#endif /* _AUTOSAR_HOOK_H */
