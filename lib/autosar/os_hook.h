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
#include "os_types.h"




OSServiceIdType _OSErrorGetServiceId;


#if OS_STARTUP_HOOK
	extern void StartUpHook(void);
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
