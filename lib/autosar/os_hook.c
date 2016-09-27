/**
 *
 *	\file os_hook.c
 *	\brief Hook implementation
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/

#include "os_hook.h"
#include "os_task.h"

#include <copperplate/internal.h>
#include <stdio.h>
#include <stdlib.h>

union autosar_wait_union {
	struct autosar_queue_wait queue_wait;
};
static DEFINE_NAME_GENERATOR(hook_namegen, "hook",struct OsHookXenomai, name);

extern struct syncluster xenomai_autosar_task_table;
static inline struct OsHookXenomai *alloc_hook(void)
{	
	return threadobj_alloc(struct OsHookXenomai,
			       thobj, union autosar_wait_union);
}
/* /
 *
*/

static void hook_finalizer(struct threadobj *__thobj)
{
	struct OsHookXenomai *__hook;	
	__hook = container_of(__thobj, struct OsHookXenomai, thobj);	
	syncluster_delobj(&xenomai_autosar_task_table, &__hook->cobj);
}

static void event_finalize(struct eventobj *__evobj)
{
	/* End of event */
	//TODO to be remove ?
}

static int hook_prologue_2(struct OsHookXenomai * __hook)
{
	int __ret;
	threadobj_wait_start();
	threadobj_lock(&__hook->thobj);
	__ret = threadobj_set_mode(0, T_LOCK, NULL);
	threadobj_unlock(&__hook->thobj);
	return __ret;
}

static void lockScheduler(struct OsHookXenomai * __hook){
	int __ret;
	threadobj_lock(&__hook->thobj);
	__ret = threadobj_set_mode(0, T_LOCK, NULL);
	if(__ret)
		warning("Failed to set mode %d",T_LOCK);
	threadobj_unlock(&__hook->thobj);
}

static void unlockScheduler(struct OsHookXenomai * __hook){
	int __ret;
	threadobj_lock(&__hook->thobj);
	__ret = threadobj_set_mode(0, 0, NULL);
	if(__ret)
		warning("Failed to set mode 0");
	threadobj_unlock(&__hook->thobj);
}

static void *hook_entry(void *__arg){
	struct OsHookXenomai * __hook = __arg;
	struct service __svc;
	unsigned int __mask = 0;
	int __evobj_mode = 0;	
	int __ret;
	unsigned int __flag = __hook->flag ;
#if OS_SHUTDOWN_HOOK | OS_ERROR_HOOK
	StatusType error = NULL;
#endif
	CANCEL_DEFER(__svc);
	__ret = __bt(hook_prologue_2(__hook));
	if (__ret) {
		CANCEL_RESTORE(__svc);
		return (void *)(long)__ret;
	}	
	threadobj_notify_entry();
	CANCEL_RESTORE(__svc);
	__evobj_mode = EVOBJ_ANY;
	
	while(1){		
		
		CANCEL_DEFER(__svc);
		__ret = eventobj_wait(&__hook->evobj, __flag, &__mask,__evobj_mode, NULL);
		if(__ret != 0 && __ret != -ETIMEDOUT)
				break;
		
		__ret = eventobj_clear(&__hook->evobj, __mask, &__mask);	
		CANCEL_RESTORE(__svc);
#if OS_SHUTDOWN_HOOK | OS_ERROR_HOOK	
		error = __hook->Error;
#endif
		lockScheduler(__hook);
#if OS_STARTUP_HOOK	
		if(__flag & OS_STARTUP_HOOK_FLAG)
			StartupHook();
#endif
#if OS_SHUTDOWN_HOOK
		if(__flag & OS_SHUTDOWN_HOOK_FLAG){
			ShutdownHook(error);
                        unlockScheduler(__hook);
                        __ret = eventobj_post(&__hook->evobj, ~__flag);
			break;
		}
#endif
#if OS_ERROR_HOOK
		if(__flag & OS_ERROR_HOOK_FLAG)
			ErrorHook(error);
#endif
#if OS_PRE_TASK_HOOK
		if(__flag & OS_PRE_TASK_HOOK_FLAG)
			PreTaskHook();
#endif
#if OS_POST_TASK_HOOK
		if(__flag & OS_POST_TASK_HOOK_FLAG)
			PostTaskHook();
#endif
		unlockScheduler(__hook);
		__ret = eventobj_post(&__hook->evobj, ~__flag);
	}
       
	return NULL;
}

static int hook_prologue(void *__arg)
{	
	struct OsHookXenomai * __hook = __arg;
	return __bt(threadobj_prologue(&__hook->thobj, __hook->name));
}

static StatusType __hookInit( char * __name,struct OsHookXenomai * __hook)
{
	struct corethread_attributes __cta;
	struct threadobj_init_data __idata;
	int __ret;
	
	generate_name(__hook->name, __name, &hook_namegen);
	__idata.magic = task_magic;
	__idata.finalizer = hook_finalizer;	
	__idata.policy = SCHED_FIFO;
	__idata.param_ex.sched_priority = 250;
	__ret = threadobj_init(&__hook->thobj, &__idata);
	if (__ret) {
		return E_OS_ID;
	}
	__ret = syncluster_addobj(&xenomai_autosar_task_table, __hook->name, &__hook->cobj);
	if (__ret) {
		threadobj_uninit(&__hook->thobj);
		return E_OS_ID;
	}

	__cta.policy = __idata.policy;
	__cta.param_ex.sched_priority = 250;
	__cta.prologue = hook_prologue;
	__cta.run = hook_entry;
	__cta.arg = __hook;
	__cta.stacksize = 0;// stacksize;
	__cta.detachstate = PTHREAD_CREATE_DETACHED;
	__ret = __bt(copperplate_create_thread(&__cta, &__hook->thobj.ptid));
	if (__ret) {
		syncluster_delobj(&xenomai_autosar_task_table, &__hook->cobj);
		threadobj_uninit(&__hook->thobj);
		return E_OS_ID;
	}
	return E_OK;
}

static struct OsHookXenomai * __hook_init(unsigned int __flag)
{
	
	struct service __svc;
	char __name[XNOBJECT_NAME_LEN];
	StatusType __ret = E_OS_ID;
	struct OsHookXenomai * __hook;
	CANCEL_DEFER(__svc);
	sprintf(__name,"Hook %u",__flag);
	__hook = alloc_hook();
	if (__hook == NULL) {
		goto out;
	}
	__ret = __hookInit(__name,__hook);
	if(__ret)
		return NULL;
	
	
out:
	CANCEL_RESTORE(__svc);

	return __hook;
}


StatusType __CreateHook(unsigned int __flag){
	StatusType __ret;
	struct OsHookXenomai * __hook = NULL;
	struct service __svc;
	int __thread_ret;
	int __evobj_flags = EVOBJ_PRIO;
	

	__hook = __hook_init(__flag);

	if (__hook == NULL) {
		__ret = E_OS_ID;
		goto out;
	}
	__hook->flag = __flag;
	CANCEL_DEFER(__svc);
	if(__flag == OS_STARTUP_HOOK_FLAG)
		OsHook_table[0] = __hook;
	else if(__flag == OS_SHUTDOWN_HOOK_FLAG)
		OsHook_table[1] = __hook;
	else if(__flag == OS_ERROR_HOOK_FLAG)
		OsHook_table[2] = __hook;
	else if(__flag == OS_PRE_TASK_HOOK_FLAG)
		OsHook_table[3] = __hook;
	else if(__flag == OS_POST_TASK_HOOK_FLAG)
		OsHook_table[4] = __hook;	
	__thread_ret = threadobj_lock(&__hook->thobj);
	__thread_ret= eventobj_init(&__hook->evobj, 0, __evobj_flags,fnref_put(libautosar, event_finalize));
	
	__thread_ret = threadobj_start(&__hook->thobj);
	switch (__thread_ret) {
	case -EIDRM:
		__ret = E_OK;
		break;
	default:
		__ret = E_OK;
	case 0:	/* == OK */
		threadobj_unlock(&__hook->thobj);
		
	}
	
out:
	CANCEL_RESTORE(__svc);
	return __ret;

}
StatusType __ActivateOsHook(unsigned int __flag, StatusType __error){
	StatusType __ret = E_OK;
	struct service __svc;
	unsigned int __mask = 0;
	int __evobj_mode = 0;
	struct OsHookXenomai * __hook = NULL;
	if(__flag == OS_STARTUP_HOOK_FLAG)
		__hook = OsHook_table[0];
	else if(__flag == OS_SHUTDOWN_HOOK_FLAG)
		__hook = OsHook_table[1];
	else if(__flag == OS_ERROR_HOOK_FLAG)
		__hook = OsHook_table[2];
	else if(__flag == OS_PRE_TASK_HOOK_FLAG)
		__hook = OsHook_table[3];
	else if(__flag == OS_POST_TASK_HOOK_FLAG)
		__hook = OsHook_table[4];
	else
		goto out;
	
	if(__hook == NULL){
		goto out;	
	}	
	CANCEL_DEFER(__svc);
	threadobj_lock(&__hook->thobj);
	__hook->Error = __error;
	__ret = eventobj_post(&__hook->evobj, __flag);
	__evobj_mode = EVOBJ_ANY;
	threadobj_unlock(&__hook->thobj);
	__ret = eventobj_wait(&__hook->evobj, ~__flag, &__mask,__evobj_mode, NULL);
	if(__ret != 0 && __ret != -ETIMEDOUT)
			goto out;
	__ret = eventobj_clear(&__hook->evobj, __mask, &__mask);
	CANCEL_RESTORE(__svc);
out : 
	return __ret;
}

void __StopHook(unsigned int __flag){
	struct service __svc;
	struct OsHookXenomai * __hook = NULL;
	if(__flag == OS_STARTUP_HOOK_FLAG)
		__hook = OsHook_table[0];
	else if(__flag == OS_SHUTDOWN_HOOK_FLAG)
		__hook = OsHook_table[1];
	else if(__flag == OS_ERROR_HOOK_FLAG)
		__hook = OsHook_table[2];
	else if(__flag == OS_PRE_TASK_HOOK_FLAG)
		__hook = OsHook_table[3];
	else if(__flag == OS_POST_TASK_HOOK_FLAG)
		__hook = OsHook_table[4];
	else
		return;
	CANCEL_DEFER(__svc);
	eventobj_destroy(&__hook->evobj);
	CANCEL_RESTORE(__svc);
	
}

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
		__ActivateOsHook(OS_ERROR_HOOK_FLAG,ret);
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
