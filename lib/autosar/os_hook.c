/**
 *
 *	\file os_hook.c
 *	\brief Hook implementation
 *	\author  Matthieu K.
 *	\version 0.1
 *	\date 29 Juin 2016
*/

#include "os_hook.h"
#include <stdio.h>
#include <stdlib.h>



#if OS_ERROR_HOOK
OSServiceIdType OSErrorGetServiceId(void){
	return _OSErrorGetServiceId;
}

/* Task ErrorHook functions */

/* Setter */
void __SetTaskID(TaskType TaskID){
	_TaskID = TaskID;
}

void __SetTaskRefID(TaskRefType TaskRefID){
	_TaskRefID = TaskRefID;
}

void __SetTaskState(TaskStateRefType State){
	_State = State;
}

/* Getter */
TaskType OSError_ActivateTask_TaskID(void){
	return _TaskID;
}

TaskType OSError_ChainTask_TaskID(void){
	return _TaskID;
}

TaskType OSError_GetTaskID_TaskID(void){
	return _TaskID;
}

TaskType OSError_GetTaskState_TaskID(void){
	return _TaskID;
}

TaskStateRefType OSError_GetTaskState_State(void){
	return _State;
}

void __CheckError(StatusType ret,OSServiceIdType ServiceId){

	if(ret){
		_OSErrorGetServiceId = ServiceId;
		ErrorHook(ret);
	}
}

/* Alarm ErrorHook functions */

/* Setters */

void __SetAlarmID(AlarmType AlarmID){
	_AlarmID = AlarmID;
}

void __SetInfo(AlarmBaseRefType Info){
	_Info = Info;
}

void __SetTick(TickRefType Tick){
	_Tick = Tick;
}

void __Setincrement(TickType increment){
	_increment = increment;
}

void __Setcycle(TickType cycle){
	_cycle = cycle;
}

void __Setstart(TickType start){
	_start = start;
}

/* Getters */
AlarmType OSError_GetAlarmBase_AlarmID(void){
	return _AlarmID;
}

AlarmBaseRefType OSError_GetAlarmBase_Info(void){
	return _Info;
}

AlarmType OSError_GetAlarm_AlarmID(void){
	return _AlarmID;
}

TickRefType OSError_GetAlarm_Tick(void){
	return _Tick;
}

AlarmType OSError_SetRelAlarm_AlarmID(void){
	return _AlarmID;
}

TickType OSError_SetRelAlarm_increment(void){
	return _increment;
}

TickType OSError_SetRelAlarm_cycle(void){
	return _cycle;
}

AlarmType OSError_SetAbsAlarm_AlarmID(void){
	return _AlarmID;
}

TickType OSError_SetAbsAlarm_start(void){
	return _start;
}

TickType OSError_SetAbsAlarm_cycle(void){
	return _cycle;
}

AlarmType OSError_CancelAlarm_AlarmID(void){
	return _AlarmID;
}

#else /* OS_ERROR_HOOK == FALSE */

OSServiceIdType OSErrorGetServiceId(void){
	return 0;
}

TaskType OSError_ActivateTask_TaskID(){
	return -1;
}

TaskType OSError_ChainTask_TaskID(){
	return -1;
}

TaskType OSError_GetTaskID_TaskID(){
	return -1;
}

TaskType OSError_GetTaskState_TaskID(){
	return -1;
}

TaskStateRefType OSError_GetTaskState_State(){
	return NULL;
}

AlarmType OSError_GetAlarmBase_AlarmID(void){
	return -1;
}

AlarmBaseRefType OSError_GetAlarmBase_Info(void){
	return NULL;
}

AlarmType OSError_GetAlarm_AlarmID(void){
	return -1;
}

TickRefType OSError_GetAlarm_Tick(void){
	return NULL;
}

AlarmType OSError_SetRelAlarm_AlarmID(void){
	return -1;
}

TickType OSError_SetRelAlarm_increment(void){
	return -1;
}

TickType OSError_SetRelAlarm_cycle(void){
	return -1;
}

AlarmType OSError_SetAbsAlarm_AlarmID(void){
	return -1;
}

TickType OSError_SetAbsAlarm_start(void){
	return -1;
}

TickType OSError_SetAbsAlarm_cycle(void){
	return -1;
}

AlarmType OSError_CancelAlarm_AlarmID(void){
	return -1;
}


#endif
