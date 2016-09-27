/**
 *
 *	\file os_task.c
 *	\brief Task implementation
 *	\author  Matthieu K.
 *	\version 1.0
 *	\date 29 Juin 2016
*/

#include <stdint.h>
#include <stdio.h>
#include <copperplate/heapobj.h>
#include <copperplate/threadobj.h>
#include <copperplate/syncobj.h>
#include <copperplate/eventobj.h>
#include <copperplate/internal.h>
#include <copperplate/registry-obstack.h>
#include <copperplate/registry.h>
#include "os_os.h"
#include "os_task.h"
#include "app/app_define.h"
#include "app/app_config.h"
#include "os_hook.h"
#include "os_timer.h"

extern struct Os os;
struct syncluster xenomai_autosar_task_table;

union autosar_wait_union {
	struct autosar_queue_wait queue_wait;
};

#ifdef CONFIG_XENO_REGISTRY

static int task_registry_open(struct fsobj *__fsobj, void *__priv)
{
	struct fsobstack *__o = __priv;
	struct threadobj_stat __buf;
	struct OsTaskXenomai *__task;
	int __ret;

	__task = container_of(__fsobj, struct OsTaskXenomai, fsobj);
	__ret = threadobj_lock(&__task->thobj);
	if (__ret)
		return -EIO;

	__ret = threadobj_stat(&__task->thobj, &__buf);
	threadobj_unlock(&__task->thobj);
	if (__ret)
		return __ret;

	fsobstack_init(__o);

	fsobstack_finish(__o);

	return 0;
}

static struct registry_operations registry_ops = {
	.open		= task_registry_open,
	.release	= fsobj_obstack_release,
	.read		= fsobj_obstack_read
};

#else /* !CONFIG_XENO_REGISTRY */

static struct registry_operations registry_ops;

#endif /* CONFIG_XENO_REGISTRY */



static void event_finalize(struct eventobj *evobj)
{
	/* End of event */
	//TODO to be remove ?
}

static DEFINE_NAME_GENERATOR(task_namegen, "task",
			     struct OsTaskXenomai, name);

static void put_OsTaskXenomai(struct OsTaskXenomai *__task)
{
	threadobj_unlock(&__task->thobj);
}

static struct OsTask * get_task(TaskType __taskID){
	struct OsTask * __osTask = NULL;
	if(__taskID <= TASK_COUNT && __taskID >= 1){	
		__osTask =  os.OsTask[__taskID-1];	
	}	
	return __osTask;	
}	


static void task_finalizer(struct threadobj *__thobj)
{
	struct OsTaskXenomai *__task;	
	__task = container_of(__thobj, struct OsTaskXenomai, thobj);	
	registry_destroy_file(&__task->fsobj);
	syncluster_delobj(&xenomai_autosar_task_table, &__task->cobj);
}


static int task_prologue_2(struct OsTaskXenomai *__task)
{
	int __ret;
	threadobj_wait_start();
	threadobj_lock(&__task->thobj);
	__ret = threadobj_set_mode(0, __task->mode, NULL);
	threadobj_unlock(&__task->thobj);
	return __ret;
}


static void lockResource(struct OsTask * __osTask){	
	struct OsResource * __osResource;
	int __i;
	/* Get Internal Resource */
	for(__i=0;__i<__osTask->ResourceCount;__i++){
		__osResource = __osTask->OsTaskResourceRef[__i];
		if(__osResource->OsResourceProperty == RESOURCE_INTERNAL)
			__GetOsResource(__osResource);		
	}
}
static void lockScheduler(struct OsTaskXenomai * __task){
	int __ret;
	threadobj_lock(&__task->thobj);
	__ret = threadobj_set_mode(0, __task->mode, NULL);
	if(__ret)
		warning("Failed to set mode %d",__task->mode);
	threadobj_unlock(&__task->thobj);
}

static void *task_entry(void *__arg){
	struct OsTask * __osTask = __arg;
	struct OsTaskXenomai *__task = __osTask->OsTaskXenomai;
	struct service __svc;
	unsigned int __mask = 0;
	int __evobj_mode = 0;	
	int __ret;
	
	CANCEL_DEFER(__svc);
	__ret = __bt(task_prologue_2(__task));
	if (__ret) {
		CANCEL_RESTORE(__svc);
		return (void *)(long)__ret;
	}	
	threadobj_notify_entry();
	CANCEL_RESTORE(__svc);
	__evobj_mode = EVOBJ_ANY;
	while(1){		
		if (__task->activation_number == 0 ){
			CANCEL_DEFER(__svc);
			__task->state = SUSPENDED;
			__ret = eventobj_wait(&__task->evobj, 1, &__mask,__evobj_mode, NULL);

			if(__ret != 0 && __ret != -ETIMEDOUT)
				break;
			__ret = eventobj_clear(&__task->evobj, 1, &__mask);	
			CANCEL_RESTORE(__svc);	
		}
#if OS_PRE_TASK_HOOK
		__ActivateOsHook(OS_PRE_TASK_HOOK_FLAG,E_OK);
#endif		/* END OF PRE HOOK */
		if(__osTask->OsTaskSchedule == TASK_FULL)
			lockScheduler(__task);
		lockResource(__osTask);
		
		__task->state = RUNNING;
		__osTask->handler();

		
	}
	return NULL;
}

static int task_prologue(void *__arg)
{
	struct OsTask * __OsTask = __arg;
	struct OsTaskXenomai *__task = __OsTask->OsTaskXenomai;
	return __bt(threadobj_prologue(&__task->thobj, __task->name));
}

static StatusType __taskInit(struct OsTask * __osTask,struct OsTaskXenomai *__task, char * __name)
{
	struct corethread_attributes __cta;
	struct threadobj_init_data __idata;
	
	
	int __ret;

	__ret = check_task_priority(__osTask->OsTaskPriority);
	if (__ret) {
		return E_OS_ID;
	}

	//TODO Check mode
	if(__osTask->OsTaskSchedule == TASK_NON){
		
		__task->mode = T_LOCK;
	}else{
		__task->mode = 0;
	}
	generate_name(__task->name, __name, &task_namegen);
	__idata.magic = task_magic;	
	__idata.finalizer = task_finalizer;
	__idata.policy = SCHED_FIFO;
	__idata.param_ex.sched_priority = __osTask->OsTaskPriority;

	__ret = threadobj_init(&__task->thobj, &__idata);
	if (__ret) {
		return E_OS_ID;
	}
	__ret = syncluster_addobj(&xenomai_autosar_task_table, __task->name, &__task->cobj);
	if (__ret) {
		threadobj_uninit(&__task->thobj);
		return E_OS_ID;
	}
	registry_init_file_obstack(&__task->fsobj, &registry_ops);
	__ret = __bt(registry_add_file(&__task->fsobj, O_RDONLY,
				     "/autosar/tasks/%s", __task->name));
	if (__ret)
		warning("failed to export task %s to registry, %s",
			__task->name, symerror(__ret));	

	__cta.policy = __idata.policy;
	__cta.param_ex.sched_priority = __osTask->OsTaskPriority;
	__cta.prologue = task_prologue;
	__cta.run = task_entry;
	__cta.arg = __osTask;
	__cta.stacksize = 0;// stacksize;
	__cta.detachstate = PTHREAD_CREATE_JOINABLE;
	
	__ret = __bt(copperplate_create_thread(&__cta, &__task->thobj.ptid));
	if (__ret) {
		registry_destroy_file(&__task->fsobj);
		syncluster_delobj(&xenomai_autosar_task_table, &__task->cobj);
		threadobj_uninit(&__task->thobj);
		return E_OS_ID;
	}
	__task->thread = __task->thobj.ptid;
	return E_OK;
}


static inline struct OsTaskXenomai *alloc_task(void)
{
	return threadobj_alloc(struct OsTaskXenomai,
			       thobj, union autosar_wait_union);
}
/* /
 *
*/
static struct OsTaskXenomai * __task_init(struct OsTask * __osTask,TaskType __taskID)
{
	struct OsTaskXenomai * __task;
	struct service __svc;
	char __name[XNOBJECT_NAME_LEN];
	StatusType __ret = E_OS_ID;

	CANCEL_DEFER(__svc);
	
	__task = alloc_task();
	if (__task == NULL) {
		goto out;
	}
	__osTask->OsTaskXenomai = __task;
	//TODO LU ne marche pour que pour 64 bits pas le 32 : trouver une autre méthode
	sprintf(__name,"Task %u",__taskID);	
	__ret = __taskInit(__osTask,__task,__name);
	if(__ret)
		return NULL;
	__task->xenomaiID = mainheap_ref(__task, TaskType);
	__task->autosarID = __taskID;
out:
	CANCEL_RESTORE(__svc);

	return __task;
}



StatusType __CreateTask(TaskType __taskID){
	struct OsTask * __osTask;
	StatusType __ret;
	struct OsTaskXenomai *__task = NULL;
	struct service __svc;
	int __thread_ret;
	int __evobj_flags = EVOBJ_PRIO;

	// Get Task from struct
	__osTask = get_task(__taskID);
	if(__osTask == NULL){
		__ret = E_OS_ID;
		goto out;
	}
	__task = __task_init(__osTask,__taskID);
	if (__task == NULL) {
		__ret = E_OS_ID;
		goto out;
	}
	
	CANCEL_DEFER(__svc);
	
	__thread_ret = threadobj_lock(&__task->thobj);
	__thread_ret= eventobj_init(&__task->evobj, 0, __evobj_flags,
			    fnref_put(libautosar, event_finalize));
	__osTask->OsTaskXenomai = __task;
	__task->state = SUSPENDED;
	__thread_ret = threadobj_start(&__task->thobj);
	switch (__thread_ret) {
	case -EIDRM:
		__ret = E_OK;
		break;
	default:
		__ret = E_OK;
	case 0:	/* == OK */
		put_OsTaskXenomai(__task);
	}
	
out:
	CANCEL_RESTORE(__svc);
	return __ret;

}
StatusType __ActivateOsTask(struct OsTask * __osTask){
	StatusType __ret = E_OK;
	struct OsTaskXenomai *__task = NULL;
	struct service __svc;
	/*unsigned int __mask = 0;
	int __evobj_mode = 0;
	*/__task = __osTask->OsTaskXenomai;
	if(__task == NULL){
#if OS_STATUS == OSOS_EXTENDED		
		__ret = E_OS_ID;
#endif
		goto out;	
	}	
	CANCEL_DEFER(__svc);
	threadobj_lock(&__task->thobj);
	if(__task->activation_number < __osTask->OsTaskActivation){
		if(__task->activation_number++ == 0)
			__ret = eventobj_post(&__task->evobj, 1);
		__task->state = READY;
	}
#if OS_STATUS == OSOS_EXTENDED
	else{		
		__ret = E_OS_LIMIT;
	}
#endif
	put_OsTaskXenomai(__task);
	
	CANCEL_RESTORE(__svc);
out : 
	return __ret;
}

StatusType __ActivateTask(TaskType __taskID){
	struct OsTask * __osTask;	
	StatusType __ret = E_OK;
	
	// Get Task from struct
	__osTask = get_task(__taskID);
	if(__osTask == NULL){
#if OS_STATUS == OSOS_EXTENDED
		__ret = E_OS_ID;
#endif
		goto out;
	}
	__ret = __ActivateOsTask(__osTask);
out : 
#if OS_ERROR_HOOK
	if(__ret){
		__SetTaskID(__taskID);
		__CheckError(__ret,OSServiceId_ActivateTask);
	}
#endif
	return __ret;
}


StatusType __TerminateTask(void){
	StatusType __ret = E_OK;
	struct OsTask * __osTask;
	struct OsResource * __osResource;
	unsigned int __mask;
	
	int __thread_ret, __clrmask;
	int __i;
	struct threadobj *__thobj = threadobj_current();
	struct OsTaskXenomai * __task;

	if (__thobj == NULL ||
	    threadobj_get_magic(__thobj) != task_magic){
#if OS_STATUS == OSOS_EXTENDED
			__ret = E_OS_CALLEVEL;
#endif
		goto out;
	}
	__task =  container_of(__thobj, struct OsTaskXenomai, thobj);
	__osTask = get_task(__task->autosarID);
	eventobj_clear(&__task->evobj, 1, &__mask); 

	for(__i=0;__i<__osTask->ResourceCount;__i++){
			__osResource = __osTask->OsTaskResourceRef[__i];
			if(__osResource->OsResourceProperty == RESOURCE_INTERNAL)
				__ReleaseOsResource(__osResource);
			if(__GetStatus(__osResource) != 0){
#if OS_STATUS == OSOS_EXTENDED
				__ret = E_OS_RESOURCE;
#endif
				goto out;
			}	
	}
	if(__osTask->OsTaskSchedule == TASK_FULL){	
		/* Disable T_LOCK to preempt the task */
		__clrmask = T_LOCK;	
		threadobj_lock(&__task->thobj);
		__thread_ret = threadobj_set_mode(__clrmask, 0, NULL);
		threadobj_unlock(&__task->thobj);
		if(__thread_ret)
			warning("Failed to unlock the scheduler");
	}
	if(__task->activation_number > 0)
			__task->activation_number--;

		
#if OS_POST_TASK_HOOK
		__ActivateOsHook(OS_POST_TASK_HOOK_FLAG,E_OK);
#endif		/* END OF POST HOOK */	

out : 
#if OS_ERROR_HOOK
	if(__ret){
		__CheckError(__ret,OSServiceId_TerminateTask);
	}
#endif
	return __ret;
}

StatusType __ChainTask(TaskType __taskID){
	StatusType __ret = E_OK;
	struct OsTask * __osTask;
	struct OsResource * __osResource;
	unsigned int __mask;
	int __thread_ret, __clrmask;
	int __i;
	struct threadobj *__thobj = threadobj_current();
	struct OsTaskXenomai * __task;

	if (__thobj == NULL ||
	    threadobj_get_magic(__thobj) != task_magic){
#if OS_STATUS == OSOS_EXTENDED
			__ret = E_OS_CALLEVEL;
#endif
		goto out;
	}
	__task =  container_of(__thobj, struct OsTaskXenomai, thobj);
	__osTask = get_task(__task->autosarID);
	eventobj_clear(&__task->evobj, 1, &__mask); 
	for(__i=0;__i<__osTask->ResourceCount;__i++){
			__osResource = __osTask->OsTaskResourceRef[__i];
			if(__osResource->OsResourceProperty == RESOURCE_INTERNAL)
				__ReleaseOsResource(__osResource);
			if(__GetStatus(__osResource) != 0){
#if OS_STATUS == OSOS_EXTENDED
					__ret = E_OS_RESOURCE;
#endif
				goto out;
			}	
	}
	if(__osTask->OsTaskSchedule == TASK_FULL){	
		/* Disable T_LOCK to preempt the task */

		__clrmask = T_LOCK;	
		threadobj_lock(&__task->thobj);
		__thread_ret = threadobj_set_mode(__clrmask, 0, NULL);
		threadobj_unlock(&__task->thobj);

		if(__thread_ret)
			warning("Failed to unlock the scheduler");
	}
	if(__task->activation_number > 0)
			__task->activation_number--;	
#if OS_POST_TASK_HOOK
		__ActivateOsHook(OS_POST_TASK_HOOK_FLAG,E_OK);
#endif		/* END OF POST HOOK */
	__ret = __ActivateTask(__taskID);
out :
#if OS_ERROR_HOOK
	if(__ret){
		__SetTaskID(__taskID);
		__CheckError(__ret,OSServiceId_ChainTask);
	}
#endif
	return __ret;
}

StatusType __Schedule(void){

	struct threadobj *__thobj = threadobj_current();
	struct OsTaskXenomai * __task;
	struct OsTask * __osTask;
	struct OsResource * __osResource;
	StatusType __ret = E_OK;
	int __thread_ret, __clrmask,__i;
	/* If not current thread or no AUTOSAR current thread No Schedule to call */
	if (__thobj == NULL || threadobj_get_magic(__thobj) != task_magic){
		__ret = E_OK;
		goto out;
	}
	__task =  container_of(__thobj, struct OsTaskXenomai, thobj);
	__osTask = get_task(__task->autosarID);
	for(__i=0;__i<__osTask->ResourceCount;__i++){
			__osResource = __osTask->OsTaskResourceRef[__i];
			if(__osResource->OsResourceProperty == RESOURCE_INTERNAL)
				__ReleaseOsResource(__osResource);
			if(__GetStatus(__osResource) != 0){
#if OS_STATUS == OSOS_EXTENDED
				__ret = E_OS_RESOURCE;
#endif
				goto out;
			}
	}
	/* Disable lock if call in preemptive task */
	if(__osTask->OsTaskSchedule == TASK_FULL){	
		/* Disable T_LOCK to preempt the task */
		__clrmask = T_LOCK;	
		threadobj_lock(&__task->thobj);
		__thread_ret = threadobj_set_mode(__clrmask, 0, NULL);
		threadobj_unlock(&__task->thobj);
	
		/* Enable T_LOCK to go back to the task */	
		__clrmask = T_LOCK;	
		threadobj_lock(&__task->thobj);
		__thread_ret = threadobj_set_mode(0, __clrmask, NULL);
		threadobj_unlock(&__task->thobj);
	}
	lockResource(__osTask);
	// TODO Check ret & StatusType => check if resource free
	if(__thread_ret){

		goto out;
	}
		
out :
#if OS_ERROR_HOOK
	if(__ret){
		__CheckError(__ret,OSServiceId_Schedule);
	}
#endif
	return __ret;
}

StatusType __GetTaskID(TaskRefType __taskID){

	struct threadobj *__thobj = threadobj_current();
	struct OsTaskXenomai * __task;
	StatusType __ret = E_OK;

	if (__thobj == NULL ||
	    threadobj_get_magic(__thobj) != task_magic){
		*__taskID = INVALID_TASK;
		goto out;
	}
	__task =  container_of(__thobj, struct OsTaskXenomai, thobj);
	*__taskID = __task->autosarID;

out :
#if OS_ERROR_HOOK
	if(__ret){
		__SetTaskRefID(__taskID);
		__CheckError(__ret,OSServiceId_GetTaskID);
	}
#endif
	return __ret;
}

StatusType __GetTaskState(TaskType __taskID, TaskStateRefType __state){
	struct OsTask * __osTask;
	struct OsTaskXenomai *__task;
	StatusType __ret = E_OK;
	__osTask = get_task(__taskID);
	if(__osTask == NULL){
#if OS_STATUS == OSOS_EXTENDED
		__ret = E_OS_ID;
#endif
		goto out;
	}
	__task = __osTask->OsTaskXenomai;
	if(__task == NULL){
#if OS_STATUS == OSOS_EXTENDED
		__ret = E_OS_ID;
#endif
		goto out;
	}
	/* if xenomai scheduler suspended the task */
	if((&__task->thobj)->status & __THREAD_S_SUSPENDED)
		*__state = SUSPENDED;
	else
		*__state = __task->state;
	
out :
#if OS_ERROR_HOOK
	if(__ret){
		__SetTaskID(__taskID);
		__SetTaskState(__state);
		__CheckError(__ret,OSServiceId_GetTaskState);
	}
#endif
	return __ret;
}

StatusType __JoinTask(TaskType __taskID){
	struct OsTask * __osTask;
	struct OsTaskXenomai *__task;
	StatusType __ret = E_OK;
	__osTask = get_task(__taskID);
	if(__osTask == NULL){
#if OS_STATUS == OSOS_EXTENDED
		__ret = E_OS_ID;
#endif
		goto out;
	}
	__task = __osTask->OsTaskXenomai;
	if(__task == NULL){
#if OS_STATUS == OSOS_EXTENDED
		__ret = E_OS_ID;
#endif
		goto out;
	}

	__ret = -__RT(pthread_join(__task->thread, NULL));
out :
	return __ret;
}

void __StopTask(TaskType __taskID){
	struct OsTaskXenomai *__task;
	struct OsTask *__osTask;
	struct service __svc;
	__osTask = get_task(__taskID);
	if(__osTask == NULL){
		return;
	}
	__task = __osTask->OsTaskXenomai;
	if(__task == NULL){
		return;
	}

	CANCEL_DEFER(__svc);
	eventobj_destroy(&__task->evobj);
	threadobj_lock(&__task->thobj);	
	threadobj_cancel(&__task->thobj);
	CANCEL_RESTORE(__svc);
}

