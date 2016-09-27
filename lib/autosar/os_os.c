/**
 *
 *	\file  os_os.c
 *	\brief Os services implementation
 *	\author  Matthieu K.
 *	\version 1.0
 *	\date 29 Juin 2016
*/
#include "os_os.h"
#include "os_task.h"
#include "os_counter.h"
#include "os_hook.h"
#include "os_resource.h"
#include "os_isr.h"
#include "Dio.h"
#include "app/app_config.h"
#include "app/app_define.h"
#include "app/app_isr.h"
#include <stdio.h>
#include <unistd.h>
#include "copperplate/internal.h"

/**
 * @ingroup autosar
 * @defgroup autosar_os_os Os management services
 *
 * All AUTOSAR Services
 *
 * @{
 */

// TODO : IF NO TASK DEFINE EMPTY FUNCTION OR REMOVE OS.H FUNCTION ?
AppModeType currentMode;
extern struct Os os;
int shutdown_state = 0;

static void initSystem (void){
#if COUNTER_COUNT > 0 && TASK_COUNT <= 0
	struct timespec rqt;
	struct service svc;
	long long ticks = 1000;
#endif
#if TASK_COUNT > 0 || ALARM_COUNT > 0 || RESOURCE_COUNT > 0
  	int i; 
#endif
  	StatusType result = E_OK;
#if RESOURCE_COUNT > 0
	for (i = 1; i <= RESOURCE_COUNT; i++)
		result = __CreateResource(i);
#endif
#if TASK_COUNT > 0
	for (i = 1; i <= TASK_COUNT; i++)
		result = __CreateTask(i);
#endif
#if ALARM_COUNT > 0
	for (i = 1; i <= ALARM_COUNT; i++)	
		result = __CreateAlarm(i); 
#endif

#if OS_STARTUP_HOOK 
	__CreateHook(OS_STARTUP_HOOK_FLAG); 
#endif
#if OS_SHUTDOWN_HOOK
	__CreateHook(OS_SHUTDOWN_HOOK_FLAG); 
#endif
#if OS_ERROR_HOOK
	__CreateHook(OS_ERROR_HOOK_FLAG); 
#endif
#if OS_PRE_TASK_HOOK
	__CreateHook(OS_PRE_TASK_HOOK_FLAG); 
#endif
#if OS_POST_TASK_HOOK
	__CreateHook(OS_POST_TASK_HOOK_FLAG); 
#endif

        __InitDio();
	if(result){
		//TODO ERROR HOOK		
	}
}	

static void startSystem (AppModeType mode){
#if TASK_COUNT > 0 || ALARM_COUNT > 0 
  int i;  
  int app_mode_mask = 1 << mode;
#endif
  StatusType result = E_OK;
#if TASK_COUNT > 0
struct OsTaskAutostart * OsTaskAutostart;
#else
#if COUNTER_COUNT > 0
        struct timespec rqt;
	struct service svc;
	long long ticks = 1000;
#endif
#endif
#if ALARM_COUNT > 0
struct OsAlarmAutostart * OsAlarmAutostart;
#endif

#if TASK_COUNT > 0
	for(i = 0; i < TASK_COUNT; i++){
		OsTaskAutostart = os.OsTask[i]->OsTaskAutostart;
	/*  each AUTOSTART task is activated if it belong to the appMode*/
		if( OsTaskAutostart != NULL && OsTaskAutostart->OsTaskAppModeRef->OsAppModeID & app_mode_mask){		
			result = __ActivateTask(i+1);
		}
	}
#endif
#if ALARM_COUNT > 0
  	for(i = 0; i < ALARM_COUNT; i++)	{
		OsAlarmAutostart = os.OsAlarm[i]->OsAlarmAutostart;
		if( OsAlarmAutostart != NULL && OsAlarmAutostart->OsAlarmAppModeRef->OsAppModeID & app_mode_mask)
			os.OsAlarm[i]->OsAlarmXenomai->isActive = TRUE;
			//result = __ActivateAlarm(i);
	}
#endif
//#if ISR_COUNT_MANUEL > 0
/*        for(i = 1; i <= ISR_COUNT_MANUEL; i++)	
		__InitISR(i);
*///#endif
#if COUNTER_COUNT > 0
	for(i = 1; i <= COUNTER_COUNT; i++)	
		__CreateCounter(i);
#endif
	if(result){
		//TODO ERROR HOOK
		
	}

#if TASK_COUNT > 0
	for(i = 1; i <= TASK_COUNT; i++){	
        	__JoinTask(i);
        }	
#else 
#if COUNTER_COUNT > 0
	while(!shutdown_state){
		clockobj_ticks_to_timeout(&autosar_clock, ticks, &rqt);	
		threadobj_sleep(&rqt);
	};
#endif
#endif
}
	
/**
 * \fn AppModeType GetActiveApplicationMode(void)
 * \brief AUTOSAR function : Get current application mode
 *
 * This service returns the current application mode. It may be
 * used to write mode dependent code.
 * 
 */
AppModeType GetActiveApplicationMode(void){	
	return currentMode;
}


/**
 * \fn void StartOS (AppModeType Mode)
 * \brief AUTOSAR function : Start the system
 *
 * The user can call this system service to start the operating
 * system in a specific mode, see chapter 5 (Spec OSEK OS 2.2.3) , Application modes.
 * 
 * \param [in] Mode application mode
 */
void StartOS (AppModeType Mode){
	currentMode = Mode;
	/* Start autostart task & alarm */
	initSystem();
#if OS_STARTUP_HOOK
	__ActivateOsHook(OS_STARTUP_HOOK_FLAG,E_OK);
#endif
	startSystem(Mode);

}
/**
 * \fn void ShutdownOS (StatusType Error)
 * \brief AUTOSAR function : Shutdown the system with error <Error>
 *
 * The user can call this system service to abort the overall
 * system (e.g. emergency off). The operating system also calls
 * this function internally, if it has reached an undefined internal
 * state and is no longer ready to run.
 * If a ShutdownHook is configured the hook routine
 * ShutdownHook is always called (with <Error> as argument)
 * before shutting down the operating system.
 * If ShutdownHook returns, further behaviour of ShutdownOS is
 * implementation specific.
 * In case of a system where OSEK OS and OSEKtime OS
 * coexist, ShutdownHook has to return.
 * <Error> needs to be a valid error code supported by OSEK OS.
 * In case of a system where OSEK OS and OSEKtime OS
 * coexist, <Error> might also be a value accepted by OSEKtime
 * OS. In this case, if enabled by an OSEKtime configuration
 * parameter, OSEKtime OS will be shut down after OSEK OS
 * shutdown.
 * 
 * \param [in] Error error occurred
 */
void ShutdownOS (StatusType Error){
        
        __StopDio();
#if OS_SHUTDOWN_HOOK
	__ActivateOsHook(OS_SHUTDOWN_HOOK_FLAG,Error);
#endif
	__StopCounter();
#if TASK_COUNT > 0 || ALARM_COUNT > 0 
  	int i; 
#endif

#if ALARM_COUNT > 0
	for (i = 1; i <= ALARM_COUNT; i++)
		__StopAlarm(i);	
#endif
#if TASK_COUNT > 0
	for (i = 1; i <= TASK_COUNT; i++)	
		__StopTask(i);
#endif
#if OS_STARTUP_HOOK 
	__StopHook(OS_STARTUP_HOOK_FLAG); 
#endif
#if OS_SHUTDOWN_HOOK
	__StopHook(OS_SHUTDOWN_HOOK_FLAG); 
#endif
#if OS_ERROR_HOOK
	__StopHook(OS_ERROR_HOOK_FLAG); 
#endif
#if OS_PRE_TASK_HOOK
	__StopHook(OS_PRE_TASK_HOOK_FLAG); 
#endif
#if OS_POST_TASK_HOOK
	__StopHook(OS_POST_TASK_HOOK_FLAG); 
#endif
	shutdown_state = 1;

}


#if TASK_COUNT > 0

/**
 * \fn StatusType ActivateTask(TaskType taskId)
 * \brief AUTOSAR function : Activate an Autosar task
 *
 * The task <TaskID> is transferred from the suspended state into
 * the ready state 14 . The operating system ensures that the task
 * code is being executed from the first statement.
 * \param [in] TaskID Task reference
 * \retval E_OK [Standard]  if no error
 * \retval E_OS_LIMIT [Standard]  Too many task activations of <TaskID>   
 * \retval E_OS_ID [Extended]  Task <TaskID> is invalid .
 */
StatusType ActivateTask(TaskType taskId){
	return __ActivateTask(taskId);
}
/**
 * \fn StatusType TerminateTask( void )
 * \brief AUTOSAR function : Terminate an Autosar task
 *
 * This service causes the termination of the calling task. The
 * calling task is transferred from the running state into the
 * suspended state
 *  
 * \retval E_OS_RESOURCE [Extended] Task still occupies resources
 * \retval E_OS_CALLEVEL [Extended] Call at interrupt level
 */
StatusType TerminateTask(void){
	return __TerminateTask();
	
}
/**
 * \fn StatusType ChainTask(TaskType taskId)
 * \brief AUTOSAR function : Terminate an Autosar task and activate a new one
 *
 * This service causes the termination of the calling task. After
 * termination of the calling task a succeeding task <TaskID> is
 * activated. Using this service, it ensures that the succeeding task
 * starts to run at the earliest after the calling task has been
 * terminated.
 * 
 * \param [in] TaskID Task reference
 * \retval E_OS_LIMIT [STANDARD] Too many task activations of <TaskID>
 * \retval E_OS_ID [Extended] Task <TaskID> is invalid
 */
StatusType ChainTask(TaskType taskId){
	return __ChainTask(taskId);
}
/**
 * \fn StatusType Schedule ( void )
 * \brief AUTOSAR function : Unlock the scheduler
 *
 * If a higher-priority task is ready, the internal resource of the task
 * is released, the current task is put into the ready state, its
 * context is saved and the higher-priority task is executed.
 * Otherwise the calling task is continued.
 * 
 * \retval E_OK [Standard]  if no error
 * \retval E_OS_CALLEVEL [Extended] Call at interrupt level
 * \retval E_OS_RESOURCE [Extended] Calling task occupies resources
 */
StatusType Schedule(void){
	return __Schedule();
}
/**
 * \fn StatusType GetTaskID(TaskRefType taskId)
 * \brief AUTOSAR function : Get the current Autosar Task ID
 *
 * GetTaskID returns the information about the TaskID of the task
 * which is currently running.
 * 
 * \param [out] TaskID Reference to the task which is currently running
 * \retval E_OK [Standard]  No error
 * \retval E_OK [Extended] No error
 */
StatusType GetTaskID(TaskRefType taskId){
	return __GetTaskID(taskId);
}
/**
 * \fn StatusType GetTaskState(TaskType TaskID, TaskStateRefType State)
 * \brief AUTOSAR function : Get the state of the task <TaskID>
 *
 * Returns the state of a task (running, ready, waiting, suspended)
 * at the time of calling GetTaskState.
 * 
 * \param [in] TaskID Reference
 * \param [out] State Reference to the state of the task <TaskID>
 * \retval E_OK [Standard]  No error
 * \retval E_OS_ID [Extended] Task <TaskID> is invalid
 */
StatusType GetTaskState(TaskType TaskID, TaskStateRefType State){
	return __GetTaskState(TaskID,State);
}
#endif

#if ALARM_COUNT > 0
/**
 * \fn StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
 * \brief AUTOSAR function : Get the base of the alarm <AlarmID>
 *
 * The system service GetAlarmBase reads the alarm base
 * characteristics. The return value <Info> is a structure in which
 * the information of data type AlarmBaseType is stored.
 * 
 * \param [in] AlarmID Reference to alarm
 * \param [out] Info Reference to structure with constants of the alarm base.
 * \retval E_OK [Standard]  No error
 * \retval E_OS_ID [Extended] Alarm <AlarmID> is invalid
 */
StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info){


	return __GetAlarmBase(AlarmID,Info);
}

/**
 * \fn StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick)
 * \brief AUTOSAR function : Get the next expiries of the alarm <AlarmID>
 *
 * The system service GetAlarm returns the relative value in ticks
 * before the alarm <AlarmID> expires.
 * 
 * \param [in] AlarmID Reference to alarm
 * \param [out] Tick Relative value in ticks before the alarm <AlarmID> expires.
 * \retval E_OK [Standard]  No error
 * \retval E_OS_NOFUNC [Standard] Alarm <AlarmID> is not used
 * \retval E_OS_ID [Extended] Alarm <AlarmID> is invalid
 */
StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick){


	return __GetAlarm(AlarmID,Tick);
}
/**
 * \fn StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle)
 * \brief AUTOSAR function : Start alarm <AlarmID> after <increment> tick
 *
 * The system service occupies the alarm <AlarmID> element.
 * After <increment> ticks have elapsed, the task assigned to the
 * alarm <AlarmID> is activated or the assigned event (only for
 * extended tasks) is set or the alarm-callback routine is called.
 * 
 * \param [in] AlarmID Reference to the alarm element
 * \param [in] increment Relative value in ticks
 * \param [in] cycle Cycle value in case of cyclic alarm. In case of single alarms,
 * cycle shall be zero
 * \retval E_OK [Standard]  No error
 * \retval E_OS_STATE [Standard] Alarm <AlarmID> is already in use
 * \retval E_OS_ID [Extended] Alarm <AlarmID> is invalid
 * \retval E_OS_VALUE [Extended] Value of <increment> outside of the admissible limits (lower than zero or greater than maxallowedvalue)
 * \retval E_OS_VALUE [Extended] Value of <cycle> unequal to 0 and outside of the admissible
 * counter limits (less than mincycle or greater than maxallowedvalue)
 */
StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle){
	return __SetRelAlarm(AlarmID,increment,cycle);
}
/**
 * \fn StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle)
 * \brief AUTOSAR function : Start alarm <AlarmID> at  <start> tick
 *
 * The system service occupies the alarm <AlarmID> element.
 * When <start> ticks are reached, the task assigned to the alarm
 * <AlarmID> is activated or the assigned event (only for extended
 * tasks) is set or the alarm-callback routine is called.
 * 
 * \param [in] AlarmID Reference to the alarm element
 * \param [in] start Absolute value in ticks
 * \param [in] cycle Cycle value in case of cyclic alarm. In case of single alarms,
 * cycle shall be zero
 * \retval E_OK [Standard]  No error
 * \retval E_OS_STATE [Standard] Alarm <AlarmID> is already in use
 * \retval E_OS_ID [Extended] Alarm <AlarmID> is invalid
 * \retval E_OS_VALUE [Extended] Value of <start> outside of the admissible limits (lower than zero or greater than maxallowedvalue)
 * \retval E_OS_VALUE [Extended] Value of <cycle> unequal to 0 and outside of the admissible
 * counter limits (less than mincycle or greater than maxallowedvalue)
 */
StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle){
	return __SetAbsAlarm(AlarmID,start,cycle);
}
/**
 * \fn StatusType CancelAlarm(AlarmType AlarmID)
 * \brief AUTOSAR function : Cancel alarm <AlarmID>
 *
 * The system service occupies the alarm <AlarmID> element.
 * When <start> ticks are reached, the task assigned to the alarm
 * <AlarmID> is activated or the assigned event (only for extended
 * tasks) is set or the alarm-callback routine is called.
 * 
 * \param [in] AlarmID Reference to an alarm
 * 
 * \retval E_OK [Standard]  No error
 * \retval E_OS_NOFUNC [Standard] Alarm <AlarmID> not in use
 * \retval E_OS_ID [Extended] Alarm <AlarmID> is invalid
 */
StatusType CancelAlarm(AlarmType AlarmID){
	return __CancelAlarm(AlarmID);
}
#endif

#if RESOURCE_COUNT > 0
/**
 * \fn StatusType GetResource ( ResourceType <ResID> )
 * \brief AUTOSAR function : Lock the resource <ResID>
 *
 * This call serves to enter critical sections in the code that are
 * assigned to the resource referenced by <ResID>. A critical
 * section shall always be left using ReleaseResource.
 * 
 * \param [in] ResID Reference to resource
 * \retval E_OK [Standard]  No error
 * \retval E_OS_ID [Extended] Resource <ResID> is invalid
 * \retval E_OS_ACCES [Extended] Attempt to get a resource which is already occupied by any task
 * or ISR, or the statically assigned priority of the calling task or
 * interrupt routine is higher than the calculated ceiling priority
 */
StatusType GetResource(ResourceType ResID){
	return __GetResource(ResID);
}
/**
 * \fn StatusType ReleaseResource( ResourceType <ResID> )
 * \brief AUTOSAR function : Unlock the resource <ResID>
 *
 * ReleaseResource is the counterpart of GetResource and
 * serves to leave critical sections in the code that are assigned to
 * the resource referenced by <ResID>
 * 
 * \param [in] ResID Reference to resource
 * \retval E_OK [Standard]  No error
 * \retval E_OS_ID [Extended] Resource <ResID> is invalid
 * \retvak E_OS_NOFUNC [Extended] Attempt to release a resource which is not occupied by any task
 * or ISR, or another resource shall be released before
 * \retval E_OS_ACCES [Extended] Attempt to release a resource which has a lower ceiling priority
 * than the statically assigned priority of the calling task or
 * interrupt routine
 */
StatusType ReleaseResource(ResourceType ResID){
	return __ReleaseResource(ResID);
}
#endif
ISRType GetISRID(void){
	return __GetISRID();
}

void EnableAllInterrupts(void){
	//__EnableAllInterrupts();
}

void DisableAllInterrupts(void){
	//__DisableAllInterrupts();
}

void ResumeAllInterrupts(void){ 
	//__ResumeAllInterrupts();
}

void SuspendAllInterrupts(void){ 
	//__SuspendAllInterrupts();
}

void ResumeOSInterrupts(void){ 
	//__ResumeOSInterrupts();
}

void SuspendOSInterrupts(void){ 
	//__SuspendOSInterrupts();
}

Dio_LevelType Dio_ReadChannel(Dio_ChannelType __ChannelId){
        return __Dio_ReadChannel(__ChannelId);
}
void Dio_WriteChannel(Dio_ChannelType __ChannelId,Dio_LevelType __Level){
        __Dio_WriteChannel(__ChannelId,__Level);
}
Dio_PortLevelType Dio_ReadPort(Dio_PortType __PortId){
        return __Dio_ReadPort(__PortId);
}
void Dio_WritePort(Dio_PortType __PortId,Dio_PortLevelType __Level){
        __Dio_WritePort(__PortId,__Level);
}
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType * __ChannelGroupIdPtr){
        return __Dio_ReadChannelGroup(__ChannelGroupIdPtr);
}
void Dio_WriteChannelGroup(const Dio_ChannelGroupType* __ChannelGroupIdPtr, Dio_PortLevelType __Level){
        __Dio_WriteChannelGroup(__ChannelGroupIdPtr,__Level);
}
void Dio_GetVersionInfo(Std_VersionInfoType * __VersionInfo){
        __Dio_GetVersionInfo(__VersionInfo);
}
Dio_LevelType Dio_FlipChannel(Dio_ChannelType __ChannelId){
        return __Dio_FlipChannel(__ChannelId);
}


#if COUNTER_COUNT > 0
/**
 * \fn StatusType IncrementCounter(CounterType CounterID)
 * \brief AUTOSAR function : Increment counter <CounterID>
 *
 * This service increments a software counter.
 * 
 * \param [in] CounterID The counter to be incremented
 * \retval E_OK [Standard]  No errors
 * \retval E_OS_ID [Extended] The CounterID was not valid or
 * counter is implemented in hardware and can not be incremented by
 * software
 */
StatusType IncrementCounter(CounterType CounterID){
	return __IncrementCounter(CounterID);
};
/**
 * \fn StatusType GetCounterValue(CounterType CounterID,TickRefType Value)
 * \brief AUTOSAR function : Get current value of counter <CounterID>
 *
 * This service reads the current count value of a counter (returning either the
 * hardware timer ticks if counter is driven by hardware or the software ticks when
 * user drives counter).This service increments a software counter.
 * 
 * \param [in] CounterID The Counter which tick value should be read
 * \retval E_OK [Standard]  No errors
 * \retval E_OS_ID [Extended] The <CounterID> was not valid
 */
StatusType GetCounterValue(CounterType CounterID,TickRefType Value){
	return __GetCounterValue(CounterID,Value);
}
/**
 * \fn StatusType GetElapsedValue(CounterType CounterID,TickRefType Value,TickRefType ElapsedValue)
 * \brief AUTOSAR function : Get the different between current value of counter <CounterID> and value <Value>
 *
 * This service gets the number of ticks between the current tick value and a
 * previously read tick value.
 * 
 * \param [in] CounterID The Counter which tick value should be read
 * \retval E_OK [Standard]  No errors
 * \retval E_OS_ID [Extended] The <CounterID> was not valid
 * \retval E_OS_VALUE [Extended] The given < Value > was not valid
 */
StatusType GetElapsedValue(CounterType CounterID,TickRefType Value,TickRefType ElapsedValue){
	return __GetElapsedValue(CounterID,Value,ElapsedValue);
}





#endif
/* @} */
